
#include "layerzero.h"
#include <stdlib.h>
#include <string.h>


size_t message_size(size_t num_proc){
    return sizeof(unsigned int)*6 + sizeof(unsigned int)*num_proc;
}

int serialize(msg_t* msg, char** buffer,size_t num_proc){
    size_t msg_size = message_size(num_proc);
    *buffer = malloc(msg_size);
    if(*buffer == NULL){
        return ERROR_MEMORY;
    }
    unsigned int temp[6 + num_proc];
    temp[0] = msg->mid;
    temp[1] = msg->ackid;
    temp[2] = msg->sender;
    temp[3] = msg->mtype;
    temp[4] = msg->urb_msg.mid;
    temp[5] = msg->urb_msg.seen_id;
    memcpy(&(temp[6]),msg->urb_msg.lcb_msg.vec_clock.vector,sizeof(unsigned int)*num_proc);
    memcpy(*buffer,temp,msg_size);
    return 0;
}

int deserialize(msg_t * msg,char* buffer,size_t num_proc){
    if(buffer == NULL){
        return ERROR_POINTER;
    }
    int error;
    unsigned int* ptr = (unsigned int*) buffer;
    msg->mid = ptr[0];
    msg->ackid = ptr[1];
    msg->sender = ptr[2];
    msg->mtype = ptr[3];
    msg->urb_msg.mid = ptr[4];
    msg->urb_msg.seen_id = ptr[5];
    error = alloc_vector(&(msg->urb_msg.lcb_msg.vec_clock),num_proc,&(ptr[6]));
    if(error){
        return error;
    }
    return 0;
}

int send_message(net_data_t* data,int socket_fd,unsigned int dest_pid,msg_t msg){
    int error;
    struct sockaddr_in address;
    error = get_addr(data->address_book,dest_pid,&address);
    if(error){
        printf("Error obtaining address \n");
        return error;
    }
    char* buffer;
    error = serialize(&msg,&buffer,data->num_proc);
    if(error){
        printf("Serialization error \n");
        return error;
    }
    error = sendto(socket_fd,(const char *) buffer,
                   message_size(data->num_proc),MSG_DONTWAIT,
                   (const struct sockaddr*) &address,
                   sizeof(struct sockaddr_in));
    if(error < 0){
        printf("Error sending \n");
        return ERROR_SEND;
    }
    if(buffer != NULL){
        free(buffer);
    }
    error = destroy_vector(&(msg.urb_msg.lcb_msg.vec_clock));
    if(error){
        printf("Destroying temporary vector clock failed \n");
        return error;
    }
    return 0;
}

int receive_msg(net_data_t* data,char* buffer, msg_t* next_message){
    int error;
    struct sockaddr_in client;
    unsigned int length_client;
    size_t msg_size = message_size(data->num_proc);
    if(buffer == NULL){
        printf("Buffer passed to receive_msg is null \n");
        return ERROR_POINTER;
    }
    error = recvfrom(data->receiver_fd,(char*) buffer,msg_size,
                     MSG_WAITALL,(struct sockaddr *) &client,&length_client);
    if(error < 0){
        printf("message reception error \n");
        return ERROR_NETWORK;
    }
    error = deserialize(next_message,buffer,data->num_proc);
    if(error){
        printf("Deserialization error \n");
        return error;
    }
    return 0;
}


int print_message(msg_t msg){
    printf("=====MESSAGE=====\n");
    printf(" mid = %u \n  ackid = %u\n sender = %u \n mtype = %u\n",msg.mid,msg.ackid,msg.sender,msg.mtype);
    printf(" urb mid = %u\n seenid = %u\n",msg.urb_msg.mid,msg.urb_msg.seen_id);
    printf("Vector clock \n");
    for(int i = 0 ; i < msg.urb_msg.lcb_msg.vec_clock.num_proc ; i++){
        printf("vec %d = %u \n",i,msg.urb_msg.lcb_msg.vec_clock.vector[i]);
    }
    return 0;
}