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
void *sender_f(void * params){
  int error;
  sender_info_t* data_s = (sender_info_t*) params;

  // WAIT TO START the following code temporarily locks a mutex (mutex gets unlocked and locked again while cond_wait)
  error = pthread_mutex_lock(data_s->start_m);
  if(error){
    fprintf(stderr,"ERROR pthread_mutex_lock() error code %d\n",error);
    return ERROR_MUTEX;
  }
  error = pthread_cond_wait(data_s->start,data_s->start_m);
  if(error){
    fprintf(stderr,"ERROR pthread_cond_wait() error code %d\n",error);
    return ERROR_CONDITION;
  }
  error = pthread_mutex_unlock(data_s->start_m);
  if(error){
    fprintf(stderr,"ERROR pthread_mutex_unlock() error code %d\n",error);
    return ERROR_MUTEX;
  }
  // FINISH WAIT TO START, the thread will now start sending its messages
  while(1){
    // send message 
  }


  return 0;
  
}



int init_senders(receiver_info_t* data){
  unsigned int no_nodes = data->no_nodes;
  // allocate array of sender_info_t 
  sender_info_t* sender_array = calloc(no_nodes,sizeof(sender_info_t));
  if(sender_array == NULL){
    fprintf(stderr,"ERROR calloc() in init_senders()");
    return ERROR_MEMORY;
  }
  // copy locally/temporarily the address book for faster access
  addr_book_t* book = data->addresses;
  unsigned int current_pid;
  // copy temporaily list of counters 
  ack_data_t* counters = data->acklist;
  int error;
  sender_info_t* curr_s = NULL;
  // iterate over each sender_info_t structure to initialize all the fields according 
  // to the data
  for(int i = 0; i < no_nodes; i++){
      // the main node id 
      curr_s = &(sender_array[i]);
      curr_s->nodeid = data->nodeid;
      // get the id of the external process corresponding to sender[i]
      current_pid = book->listaddr[i].process_id;
      // set all the other fields accordingly
      curr_s->process_address->process_id = current_pid;
      curr_s->process_address->address = book->listaddr[i].address;
      curr_s->queue = &(data->thread_queues->queues[i]);
      curr_s->queue->pid = current_pid;
      curr_s->ack_counter = &(counters->acks[i]);
      curr_s->ack_counter->pid = current_pid;
      // copy the address of the condition mutex to start sending messages
      curr_s->start = data->start;
      curr_s->start_m = data->start_m;
      // when everything is done initialize the socket (so that the filedescriptor of the sender gets updated too)
      error = init_socket_sender(curr_s);
      if(error){
        return error;
      }
      // everything is set up so we can now spawn the corresponding thread
      error = pthread_create(&data->senders[i],NULL,sender_f,(void*) curr_s);
       
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
