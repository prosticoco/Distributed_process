#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include "error.h"
#include "data.h"
#include "layers.h"
#include "fifo.h"
#include "log.h"



#define MAX_SIZE_PENDING 1024*8


int add_pending(pending_t* pending,unsigned int seen_id,unsigned int value){
    int error;
    if(seen_id > MAX_SIZE_PENDING){
        return ERROR_TABLE;
    }
    while(seen_id >= pending->total_entries){
        error = realloc_pending(pending);
        if(error){
            return error;
        }    
    }
    pending->bol[seen_id] = value;
    return 0;
}

int get_pending(pending_t* pending, unsigned int seen_id){
    if(seen_id >= pending->total_entries){
        return 0;
    }
    return pending->bol[seen_id];
}

int free_pending(pending_t* pending){
    free(pending->bol);
    pending->bol = NULL;
    return 0;

}

int init_pending(pending_t* pending, unsigned int no_msgs, unsigned int no_process){
    pending->total_entries = no_msgs*no_process;
    pending->bol = calloc(pending->total_entries,sizeof(uint32_t));
    if(pending->bol == NULL){
        return ERROR_MEMORY;
    }
    pending->no_msgs = no_msgs;
    pending->no_process = no_process;
    return 0;
}

int realloc_pending(pending_t* pending){
    unsigned int old_size = pending->total_entries;
    unsigned int new_size = pending->total_entries*2;
    pending->bol = realloc(pending->bol,new_size);
    if(pending->bol == NULL){
        return ERROR_MEMORY;
    }
    memset(&(pending->bol[old_size]),0,new_size - old_size);
    pending->total_entries *= 2;
    pending->no_msgs *= 2;
    return 0;
}

int init_next(next_t* next,unsigned int no_process){
    next->entries = calloc(no_process,sizeof(unsigned int));
    if(next->entries == NULL){
        return ERROR_MEMORY;
    }
    next->no_process = no_process;
    return 0;
}

int get_next(next_t* next, unsigned int pid){
    if(pid > next->no_process){
        return ERROR_TABLE;
    }
    return next->entries[pid -1] +1;
}

int incr_next(next_t* next,unsigned int pid){
    if(pid > next->no_process){
        return ERROR_TABLE;
    }
    next->entries[pid -1] += 1;
    return 0;
}

int free_next(next_t* next){
    free(next->entries);
    next->entries = NULL;
    return 0;
}


int send_fifo(net_data_t* data, int m){
    unsigned int self = data->self_pid;
    for(size_t i = 1; i < m+1; ++i){
        fifo_msg_t msg;
        msg.original_sender = self;
        msg.sequence_num = i;
        send_urb(data, msg);
    }
    return 0;
}


int deliver_fifo(net_data_t* data, fifo_msg_t msg){
    unsigned int seen_id = (data->num_proc) * (msg.sequence_num-1) + msg.original_sender-1;
    int error;
    error = add_pending(data->pending, seen_id,1);
    if(error <0){
        printf("Error adding to pending set, FIFO deliver\n");
        return error;
    }
    unsigned int next_idx;
    // PRINT FOR DEBUGGING
    if(data->self_pid == 1){
        //printf("PID : %zu **MESSAGE WITH SEQUENCE NUM 1**\n",data->self_pid);
        //printf("sequence = %u \n",msg.sequence_num);
    }
    next_idx = get_next(data->next,msg.original_sender);
    size_t i = 0;
    while(get_pending(data->pending, (data->num_proc) * (next_idx-1) + msg.original_sender-1) == 1){
        if(data->self_pid == 1){
            printf(" i : %zu\n",i);
        }
        error = incr_next(data->next,msg.original_sender);
        if(error <0){
            return error;
        }
        error = add_pending(data->pending,(data->num_proc) * (next_idx-1) + msg.original_sender -1, 0);
        if(error<0){
            return error;
        }
        //write to log
        error = log_deliver(data,msg);
        if(error){
            return error;
        }
        next_idx = next_idx+1;
        i++;
        
    }
    return 0;
}

int log_deliver(net_data_t* data,fifo_msg_t msg){
    unsigned int snr = msg.sequence_num;
    unsigned int pid = msg.original_sender;
    char line[LINE_MAX_LENGTH];
    int error = sprintf(line,"d %u %u\n",pid,snr);
    if(error < 0){
        return ERROR_IO;
    }
    size_t size = strlen(line);
    error =write_log(data->logdata,line,size);
    if(error){
        return error;
    }
    return 0;
}