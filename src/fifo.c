#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>

#include "error.h"
#include "data.h"
#include "layers.h"


int add_pending(pending_t* pending,unsigned int seen_id);

int get_pending(pending_t* pending, unsigned int seen_id);

int free_pending(pending_t* pending);

int init_pending(pending_t* pending, unsigned int no_msgs, unsigned int no_process);


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
    unsigned int seen_id = (data->address_book->num_proc-1) * msg.sequence_num + msg.original_sender-1;
    int error;
    error = add_pending(data->pending, seen_id);
    if(error <0){
        return error;
    }
    unsigned next_idx;
    next_idx = get_next(msg.original_sender);
    
    while(error = get_pending((data->address_book->num_proc-1) * next_idx + msg.original_sender-1) == 1){
        error = write_next(msg.original_sender, next_idx+1);
        if(error <0){
            return error;
        }
        error = write_pending((data->address_book->num_proc) * next_idx + msg.original_sender, 0);
        if(error<0){
            return error;
        }
        //write to log
        //error = deliver(BUFFER TYPE BUFFERmsg.original_sender, next_idx);
        next_idx = next_idx+1;
        
    }
    return 0;
}