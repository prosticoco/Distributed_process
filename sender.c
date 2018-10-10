#include "sender.h"
#include "error.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>

// callback function for sender thread, ie main function for thread
void *sender_f(void * params);



int init_senders(receiver_info_t* data){
  unsigned int no_nodes = data->no_nodes;
  sender_info_t* senders = calloc(no_nodes,sizeof(sender_info_t));
  if(senders == NULL){
    fprintf(stderr,"ERROR calloc() in init_senders()");
    return ERROR_MEMORY;
  }
  addr_book_t* book = data->addresses;
  unsigned int current_pid;
  ack_data_t* counters = data->acklist;
  int error;
  for(int i = 0; i < no_nodes; i++){
      senders[i].nodeid = data->nodeid;
      current_pid = book->listaddr[i].process_id;
      senders[i].process_address->process_id = current_pid;
      senders[i].process_address->address = book->listaddr[i].address;
      senders[i].queue = &(data->thread_queues->queues[i]);
      senders[i].queue->pid = current_pid;
      senders[i].ack_counter = &(counters->acks[i]);
      senders[i].ack_counter->pid = current_pid;
      senders[i].fd = NULL;
      error = init_socket_sender(&senders[i]);
      if(error){
        return error;
      }
      // TO FINISH 
  }
}

int init_socket_sender(sender_info_t* data){
  // open a new socket
  int desc = socket(AF_INET,SOCK_DGRAM,0);
  // check if any error
  if(desc < 0){
    return ERROR_SOCKET;
  }
  // update the thread's file descriptor
  data->fd = desc;
  return 0;
}

int send_pl(sender_info_t* data,msg_t* msg){

  int error = 0;

  while(read_ack(msg->msg_nr, data->ack_counter) == 0){
    error = send_fl(data->fd, data->process_address->address, msg);
    if(error < 0){
      return ERROR_SEND;
    }
    sleep(50);
  }
}


int send_fl(int fd,struct sockaddr_in* address,msg_t* msg){
  int error;
  // sends a message to the corresponding address pointed by data
  error = sendto(fd,(const char*) msg, sizeof(msg_t),
                 MSG_DONTWAIT,
                 (const struct sockaddr *) address,
                  sizeof(struct sockaddr_in));
  // handles any error due to sendto
  if(error < 0){
    return ERROR_SEND;
  }
  return 0;
}

//simple best effort broadcast uses pl_send
int broadcast_beb(msg_t* msg, addr_book_t* book);


//unified reliable broadcast that uses beb
int broadcast_urb(msg_t* msg, addr_book_t* book){

  /**pending.add(self,msg->msg_nr)
   * broadcast_beb(msg);
   */

}


int terminate_senders(sender_info_t* senders,size_t no_senders){
  int error;
  // iterate over all file descriptors and close the sockets
  for(int i = 0 ; i < no_senders; i++){
    error = close(senders[i].fd);
    if(error < 0){
      return ERROR_CLOSE;
    }
  }
  return 0;
}
