#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "error.h"
#include "layers.h"
#include "mqueue.h"
#include "receiver.h"
#include "addrbook.h"

void *receiver_f(void* params){
    int dump;
    if (!pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump)) {
        pthread_exit((void *) ERROR_THREAD);
    }
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


// code that initialize (and spawns !) the receiver thread which acts like a server
int init_receiver(net_data_t* data){
    int error = pthread_mutex_init(&(data->rec_mutex),NULL);
    if(error){
        return ERROR_MUTEX;
    }
    
    int rec_socket_fd;
    struct sockaddr_in receiver_addr;
    error = get_addr(data->address_book,data->self_pid,&receiver_addr);
    if(error){
        return error;
    }
    rec_socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(rec_socket_fd < 0){
        return ERROR_SOCKET;
    }
    data->receiver_fd = rec_socket_fd;
    error = bind(rec_socket_fd,(const struct sockaddr*)&receiver_addr,sizeof(receiver_addr));
    if(error < 0){
        return ERROR_BIND;
    }
    error = pthread_create(&(data->receiver),NULL,receiver_f,data);
    if(error){
        return ERROR_THREAD;
    }
    printf(" \n");
    printf("Thread creation succeeded for PID : %u\n",data->self_pid);
    printf(" \n");
    return 0;
}

int terminate_receiver(net_data_t* data){
    int error;
    pthread_mutex_lock(&(data->logdata->loglok));
    error = pthread_cancel(data->receiver);
    if(error){
        fprintf(stderr,"Could not cancel Receiver of pid : %zu \n",data->self_pid);
        printf("Error message %s \n",strerror(error));
        return ERROR_THREAD;
    }
    pthread_join(data->receiver,NULL);
    pthread_mutex_unlock(&(data->logdata->loglok));
    return 0;
}