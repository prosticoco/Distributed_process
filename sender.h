#ifndef SENDER_H
#define SENDER_H

#include "structure.h"

// callback function for sender thread, ie main function for thread
void *sender_f(void * params);


// function called by spawn_senders to create socket for 1 sender
int init_socket_sender(sender_info_t* data);

// initialize everything for sender threads
int init_senders(receiver_info_t* data);

// sends a message msg via perfect link
int send_pl(sender_info_t* data,msg_t* msg);

// sends a message msg via fair loss link
int send_fl(int fd,struct sockaddr_in* address,msg_t* msg);

/**
 * the broadcast method is called by a process which then distributes pl_sends to threads 
 * the sender_info_t* data is dependant on the thread so it is constructed 
 * after/inside the broadcast call
 */

//simple best effort broadcast uses pl_send
int broadcast_beb(msg_t* msg, addr_book_t* book);


//unified reliable broadcast that uses beb
int broadcast_urb(msg_t* msg, addr_book_t* book);


// called when program stops, closes the senders file descriptors, senders is a pointer to a list of senders
int terminate_senders(receiver_info_t* data);


#endif
