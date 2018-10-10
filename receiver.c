#include "receiver.h"
#include "error.h"
#include "mqueue.h"
#include "addrbook.h"
#include "ack.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>




// function callback for receiver threads, each spawned receiver will call this function and start reading msgs
void *receiver_f(void * params){
    msg_t next_message;
    int error;
    sender_info_t* data = (sender_info_t *) params;
    struct sockaddr_in client;
    unsigned int length_client;
    while(1){
        error = recvfrom(data->fd,(char *) &next_message,sizeof(msg_t),
        MSG_WAITALL, (struct sockaddr *) &client,&length_client);
        if(error < 0){
            fprintf(stderr,"ERROR : recvfrom return error code %d \n",error);
            return ERROR_NETWORK;
        }
        error = deliver(data,&client,&next_message);
        if(error < 0){
            fprintf(stderr,"ERROR deliver() error code  %d\n",error);
            return error;
        }
    }

}
// creates a socket for receiving messages, updates the value of file descriptor
// returns <0 on error, 0 otherwise
int init_receiver_socket(int * rec_fd,struct sockaddr_in* my_addr){
    *rec_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(*rec_fd < 0){
        return ERROR_SOCKET;
    }
    if(bind(*rec_fd,(struct sockaddr*) my_addr,sizeof(struct sockaddr_in)) < 0){
        return ERROR_BIND;
    }
    return 0;
}

// spawns receiver thread 
int init_receiver(receiver_info_t* data){
    int error;
    // initialize the receiver's socket
    error = init_receiver_socket(&(data->fd),data->my_address);
    if(error){
        return error;
    }
    error = pthread_create(data->receiver,NULL,receiver_f,data);
    if(error){
        fprintf(stderr,"ERROR pthread_create() return code : %d \n",error);
        return ERROR_THREAD;
    }
    return 0;
}


int end_receiver(receiver_info_t* data){
    if(close(data->fd)){
        return ERROR_IO;
    }
    if(pthread_join(data->receiver,NULL)){
        return ERROR_THREAD;
    }
    return 0;
    
}

