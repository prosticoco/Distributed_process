#ifndef SENDER_H
#define SENDER_H

#include "structure.h"

// callback function for sender thread, ie main function for thread
void *sender_f(void * params);

// called by main thread to spawn the sender threads, params are to change
int spawn_senders(void * params);

// function called by spawn_senders to create socket for 1 sender
int init_socket_sender(sender_info_t* data);



// sends a message msg via perfect link
int send_pl(sender_info_t* data,msg_t* msg);

// sends a message msg via fair loss link
int send_fl(int fd,struct sockaddr_in* address,msg_t* msg);


// called when program stops, closes the senders file descriptors, senders is a pointer to a list of senders
int terminate_senders(sender_info_t* senders,size_t no_senders);

#endif