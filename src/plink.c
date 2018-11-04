#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "plink.h"
#include "mqueue.h"
#define MAX_SIZE 1024*8
#define THRESHOLD 50

pthread_mutex_t ack_mutex = PTHREAD_MUTEX_INITIALIZER;

int init_table_uid(uid_table_t* table,unsigned int no_msgs, unsigned int no_process){
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
    unsigned int old_size = table->total_entries;
    if(mid > MAX_SIZE){
        return ERROR_TABLE;
    }
    while(mid >= table->total_entries){
        new_size = table->total_entries * 2;
        table->entries = realloc(table->entries,sizeof(unsigned int)*new_size);
        if(table->entries == NULL){
            return ERROR_MEMORY;
        }     
        table->total_entries = new_size;
        memset(&(table->entries[old_size]),0,table->total_entries - old_size);
        table->no_msgs = table->no_msgs*2;
        return 0;
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

int pl_send(unsigned int pid,net_data_t* data, msg_t* msg){
    int error = 0;
    int i = 0;
    while (!is_ack(data->pl_acks, msg->mid)) {
        if(i >= THRESHOLD){
            queue_task_t* task = malloc(sizeof(queue_task_t));
            
            error = enqueue(data->task_q, queue_task_t{pid, msg});
        }
        i+= 1;
        error = send_fl(data->fd, &(data->process_address->address), msg);
        printf("just sent a message via fair loss link, what a story mark\n");
        if (error < 0){
            return ERROR_SEND;
        }
        usleep(2000);
    }
    printf("exiting perfect link, number of iterations in while loop = %d\n",i);
    return 0;
}

int pl_deliver(net_data_t* data, msg_t* msg);