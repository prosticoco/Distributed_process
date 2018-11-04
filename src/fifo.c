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

}