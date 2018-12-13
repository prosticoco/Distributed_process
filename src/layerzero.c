
#include "layerzero.h"
#include <stdlib.h>
#include <string.h>



int serialize(msg_t* msg, char** buffer,size_t num_proc){
    *buffer = malloc(sizeof(unsigned int)*6 + sizeof(unsigned int)*num_proc);
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
    memcpy(*buffer,temp,sizeof(unsigned int)*(6 + num_proc));
    return 0;
}

int deserialize(msg_t * msg,char* buffer,size_t num_proc){
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

int print_message(msg_t msg){
    printf("=====MESSAGE=====\n");
    printf(" mid = %u \n  ackid = %u\n sender = %u \n mtype = %u\n",msg.mid,msg.ackid,msg.sender,msg.mtype);
    printf(" urb mid = %u\n seenid = %u\n",msg.urb_msg.mid,msg.urb_msg.seen_id);
    printf("Vector clock \n");
    for(int i = 0 ; i < msg.urb_msg.lcb_msg.vec_clock.num_proc ; i++){
        printf("vec %d = %u \n",msg.urb_msg.lcb_msg.vec_clock.vector[i]);
    }
    return 0;
}