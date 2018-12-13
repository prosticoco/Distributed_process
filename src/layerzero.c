
#include "data.h"
#include "error.h"
#include "pending.h"
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
    

}