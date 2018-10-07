#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

// file representing all relevant structures for the project, can be changed....

// NOTE : we need a way to signal sender_threads when they can start sending
//        we should agree on which way to signal the threads (static variable or pointer)

// parameter to define maximum text message length
#define MAX_MESSAGE_LENGTH 256
/**

structure defining a message to be sent via UDP
*/
typedef struct {
  size_t msg_type; //if ack or not
  unsigned int* msg_nr;
  unsigned int* src_id;
  char mtext[MAX_MESSAGE_LENGTH];
} msg_t;

// represents ACK Matrix (à changer si besoin @droz)
typedef bool** ack_matrix_t;

typedef bool* ack_list_t;

// represents an element of the queue, will just be equal to the msg_nr
typedef unsigned int* queue_elem_t;

// structure representing a queue of messages to be sent. 1 per sender_thread
typedef struct {
  // the number of elements in the queue
  size_t no_elem;
  // the size of the queue
  size_t qsize;
  // index corresponding to the front of the queue
  unsigned int front;
  // index corresponding to the back of the queue
  unsigned int back;
  // pointer to the elements of the queue
  queue_elem_t* elems;
} msg_queue_t;

// this structure is just used to associate a thread with its own message queue
typedef struct {
  // the id of the sender_thread
  unsigned int thread_id;
  // the msg queue associated with a thread
  msg_queue_t* queue;
} thread_msg_queue_t;

// structure representing the list of message queues, 1 for all threads
typedef struct {
  // total number of sender threads
  size_t no_sender;
  thread_msg_queue_t* queues;
} queue_list_t;


// structure to represent another process and its respective address
// we still have to choose if we will just use a list or a structure for the address list
typedef struct {
  unsigned int* process_id;
  // address structure for sending messages over UDP
  struct sockaddr_in* address;
} addr_entry_t;

// made a typedef for the address list, can be changed, I thought it might be useful
// to create an interface for handling it
typedef struct{
  size_t size;
  addr_entry_t* listaddr;
}addr_book_t;

// structure representing all the information a sender_tread will have access to
typedef struct {
  // unique id for sender_thread (might be set as the same as process_id)
  unsigned int* thread_id;
  // Address and unique id of the associated process
  addr_entry_t* process_address;
  // the sender_thread's msg queue
  thread_msg_queue_t* queue;
  //pointer to the ack matrix
  ack_matrix_t* acks;
} sender_info_t;


// structure reprenting all info a receiver thread needs, might be subject to change
// in the case where we add a new thread responsible for sending acks
typedef struct {
  // pointer to the address book of other processes
  addr_book_t* addresses;
  // pointer to the list of thread queues
  queue_list_t* thread_queues;
  // pointer to ack matrix
  ack_matrix_t* ack_matrix;

} receiver_info_t;

/**
 * @brief Structure defining a process. Either the current one, or one to which we want
 *        to send data.
 *
 */
typedef struct da_process {
    // Unique process identifier for our program
    int uid;
    // 4 bytes, 1 byte per element of the address
    struct in_addr ipv4_addr;
    // Port number on which the process listens for incoming messages
    in_port_t port_num;
} da_process_t;

#endif
