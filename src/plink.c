#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "layers.h"
#include "mqueue.h"
#include "data.h"

#define MAX_SIZE 1024*8
#define THRESHOLD 50
#define ACK_NO 1

pthread_mutex_t ack_mutex = PTHREAD_MUTEX_INITIALIZER;

int init_table_uid(uid_table_t* table, unsigned int no_msgs, unsigned int no_process){
    table->entries = calloc(no_msgs*no_process*no_process,sizeof(unsigned int));
    if(table->entries == NULL){
        return ERROR_MEMORY;
    }
    table->no_msgs = no_msgs;
    table->no_process = no_process;
    table->total_entries = no_msgs*no_process*no_process;
    return 0;
}

int table_read_entry(uid_table_t* table,mid_t mid){
    if(mid >= table->total_entries){
        return 0;
    }
    return table->entries[mid];
}

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

// Perfect link ack table methods
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
    free(&(acks->table));
    return 0;
}

// Interface for delivered table

int init_deliver_pl(pl_delivered_t* delivered,unsigned int no_entries, unsigned int no_process);

int is_delivered(pl_delivered_t* delivered,mid_t mid);

int set_delivered(pl_delivered_t* delivered,mid_t mid);

int free_delivered(pl_delivered_t* delivered);



int send_pl(unsigned int pid, size_t thread_idx, net_data_t* data, msg_t msg) {
    int error = 0;
    int i = 0;
    while (!is_ack(data->pl_acks, msg.mid)) {
        if(i >= THRESHOLD){
            queue_task_t task;
            task.pid_dest = pid;
            task.msg = msg;
            error = enqueue(data->task_q, &task);
            if(error < 0){
                return error;     
            }
            return 0;
        }
        error = send_fl(data, thread_idx, pid, msg);
        i+= 1;
        if (error < 0){
            return ERROR_SEND;
        }
        usleep(2000);
    }
    return 0;
}

int deliver_pl(net_data_t* data, size_t thread_idx, msg_t msg){
    int error = 0;
    //IF ACK add ack to acklist. basta.
    if (msg.mtype == ACK_NO) {
        error = set_ack(data->pl_acks, msg.mid); 
        if (error < 0) {
            return error;
        }
    }
    //real message
    else {
        //create ack message      
        msg_t ack;
        ack.mid = msg.mid;
        ack.mtype = ACK_NO;
        ack.sender = data->self_pid;
        
        error = deliver_beb(data, msg.urb_msg);
        if(error < 0){
            return error;
        }
        error = send_fl(data, thread_idx, msg.sender, ack);
        if (error < 0) {
            return error;
        }
        
    }
    return 0;

}