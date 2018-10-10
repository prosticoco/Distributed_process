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
