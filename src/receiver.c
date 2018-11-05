#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#include "data.h"
#include "error.h"
#include "layers.h"
#include "mqueue.h"
#include "receiver.h"

void *receiver_f(void* params){
    msg_t next_message;
    int error;
    net_data_t* data = (net_data_t *) params;
    struct sockaddr_in client;
    unsigned int length_client;
    while (1) {
        error = recvfrom(data->receiver_fd, (char *) &next_message, sizeof(msg_t),
            MSG_WAITALL, (struct sockaddr *) &client, &length_client);
        if (error < 0) {
            fprintf(stderr,"ERROR : recvfrom return error code %d \n", error);
            exit(ERROR_NETWORK);
        }
        error = process_msg(data,next_message);
        if (error < 0) {
            fprintf(stderr, "ERROR while processing msg  %d\n", error);
            exit(error);
        }
    }
    return NULL;
}

int process_msg(net_data_t* data, msg_t msg){
    int error = deliver_fl(data,msg);
    return error;
}

int init_receiver(net_data_t* data){
    return 0;
}