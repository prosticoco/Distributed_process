#ifndef RECEIVER_H
#define RECEIVER_H

#include "structure.h"

// function callback for receiver threads, each spawned receiver will call this function and start reading msgs
void *receiver_f(void * params);

// creates a socket for receiving messages, updates the value of file descriptor
// returns <0 on error, 0 otherwise
int init_receiver_socket(int * rec_fd,struct sockaddr_in* my_addr);

// spawns receiver thread 
int init_receiver(receiver_info_t* data);



int deliver(receiver_info_t* data,struct sockaddr_in* sender,msg_t* msg);

int end_receiver(receiver_info_t* data);



#endif
