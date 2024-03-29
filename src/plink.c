#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "layers.h"
#include "mqueue.h"
#include "data.h"
#include "pending.h"

#define MAX_SIZE 1024*8
#define THRESHOLD 10
#define ACK_NO 1


/**
 * @brief PERFECT LINK LAYER USING FAIR LOSS
 * for every message received we resend an ack to tell the sender we got their message
 * the sender while not having received the ack, continues sending fair loss messages every
 * delta time.
 * The sender being responsible to send to multiple processes cannot block on one message
 * so if after a certain threshold of loops still doesnt get the ack, it puts the task
 * of sending the message to that process at the end of a task queue and starts sending
 * the next message.
 */



pthread_mutex_t ack_mutex = PTHREAD_MUTEX_INITIALIZER;

//-----------general table used for ack table and delivered table-----
int init_table_uid(uid_table_t* table, unsigned int no_msgs, unsigned int no_process){
    table->entries = calloc(no_msgs*no_process*no_process,sizeof(unsigned int));
    if(table->entries == NULL){
        printf("Error Allocating ACK Table\n");
        return ERROR_MEMORY;
    }
    table->no_msgs = no_msgs;
    table->no_process = no_process;
    table->total_entries = no_msgs*no_process*no_process;
    return 0;
}
//returns value at mid, 0 if not in table
int table_read_entry(uid_table_t* table,mid_t mid){
    if(mid >= table->total_entries){
        return 0;
    }
    return table->entries[mid];
}
//add value at entry mid, if table too small we realloc table size
int table_write_entry(uid_table_t* table,mid_t mid,unsigned int value){
    unsigned int new_size;
    if(mid > MAX_SIZE){
        return ERROR_TABLE;
    }
    while(mid >= table->total_entries){
        unsigned int old_size = table->total_entries;
        new_size = table->total_entries * 2;
        table->entries = realloc(table->entries,sizeof(unsigned int)*new_size);
        if(table->entries == NULL){
            printf("realloc did not work \n");
            return ERROR_MEMORY;
        }        
        memset(&(table->entries[old_size]),0,new_size - old_size);
        table->total_entries = new_size;
        table->no_msgs = table->no_msgs*2;
    }
    table->entries[mid] = value;
    return 0;
}
int free_table_uid(uid_table_t* table){
    free(table->entries);
    table->entries = NULL;
    return 0;
}
//-----------general table used for ack table and delivered table-----

//-----------Perfect link ack table methods---------------------------
int init_ack_table(ack_table_t* acks, unsigned int no_entries, unsigned int no_process){
    int error = pthread_mutex_init(&(acks->table_mutex),NULL);
    if(error){
        return ERROR_MUTEX;
    }
    error = init_table_uid(&(acks->table),no_entries, no_process);
    if(error){
        return error;
    }
    return 0;
}

int is_ack(ack_table_t* acks,mid_t mid){
    pthread_mutex_lock(&(acks->table_mutex));
    int value = table_read_entry(&(acks->table),mid);
    pthread_mutex_unlock(&(acks->table_mutex));
    return value;
}

int set_ack(ack_table_t* acks,mid_t mid){
    pthread_mutex_lock(&(acks->table_mutex));
    int error = table_write_entry(&(acks->table),mid,1);
    pthread_mutex_unlock(&(acks->table_mutex));
    return error;
}

int free_ack_table(ack_table_t * acks){
    free_table_uid(&(acks->table));
    return 0;
}
//-----------Perfect link ack table methods---------------------------

//-----------Interface for delivered table----------------------------
int init_deliver_pl(pl_delivered_t* delivered,unsigned int no_entries, unsigned int no_process){
    int error = init_table_uid(&(delivered->table),no_entries, no_process);
    if(error){
        printf("Error initializing delivered table\n");
        return error;
    }
    return 0;
}
int is_delivered(pl_delivered_t* delivered,mid_t mid){
    return table_read_entry(&(delivered->table),mid);
}
int set_delivered(pl_delivered_t* delivered,mid_t mid){
    int error = table_write_entry(&(delivered->table),mid,1);
    if(error){
        printf("Error writing in delivered table\n");
        return error;
    }
    return 0;
}
int free_delivered(pl_delivered_t* delivered){
    free(delivered->table.entries);
    return 0;
}
//-----------Interface for delivered table----------------------------


//Perfect Link send
/* We check ack table (if we got a response for our message) every 200t
*  and we resend our message with fair loss
*  If after 10 (THRESHOLD) loops we still didnt get the ack
*  we put the task of sending this message to the end of the queue and 
*  start sending next message
*/
int send_pl(unsigned int pid, int socket_fd, net_data_t* data, msg_t msg) {
    int error = 0;
    int i = 0;
    while (!is_ack(data->pl_acks, msg.ackid)) {
        if(i >= THRESHOLD){
            queue_task_t task;
            task.pid_dest = pid;
            task.msg = msg;
            error = enqueue(data->task_q, &task);
            if(error < 0){
                printf("Error enqueuing send_pl\n");
                return error;     
            }
            return BACK_TO_QUEUE;
        }
        error = send_fl(data, socket_fd, pid, msg);
        i+= 1;
        if (error < 0){
            printf("Error while sending in send_pl\n");
            return ERROR_SEND;
        }
        usleep(200);
    }
    return 0;
}

//PERFECT LINK DELIVER
/* Whenever we receive a message we check if it is an ack or not (MTYPE)
 * If the message is an ack we update our ack table and were done
 * 
 * If the message is real we send an ack to the sender (even if the message we received was
 * already delivered. this just means the ack was lost)
 * After sending the ack we check if the message was delivered i.e. check the delivered uid table
 * If already delivered, were done
 * If not, we set it as delivered and we deliver the message i.e. call the next (upper) layer: beb
 */
int deliver_pl(net_data_t* data, msg_t msg){

    int error = 0;
    if (msg.mtype == ACK_NO) {
        error = set_ack(data->pl_acks, msg.ackid); 
        if (error < 0) {
            printf("Error while setting ACK Error num %d\n",error);
            return error;
        }
    }else{
        //it is a real message
        //create ack message     
        msg_t ack;
        //ack.mid = msg.mid - (msg.sender -1) + (data->self_pid -1);
        ack.mid = msg.mid;
        ack.ackid = msg.ackid;
        ack.mtype = ACK_NO;
        ack.sender = data->self_pid;
        error = send_fl(data, data->receiver_fd, msg.sender, ack);
        if (error < 0) {
            printf("Error send_fl in deliver_pl\n");
            return error;
        }
        if(!is_delivered(data->pl_delivered,msg.mid)){
            error = set_delivered(data->pl_delivered,msg.mid);
            if(error){
                printf("Error set_delivery\n");
                return error;
            }
            error = deliver_beb(data, msg.urb_msg);
            if(error < 0){
                printf("Error deliver beb in deliver pl\n");
                return error;
            }
        }     
    }
    return 0;
}