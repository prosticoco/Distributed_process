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
#define MESSAGE_LENGTH 256
/**

structure defining a message to be sent via UDP
*/

typedef unsigned int uint;
typedef struct{
  size_t msg_type; //if ack or not
  uint msg_nr;
  uint srcId;
  char mtext[MESSAGE_LENGTH];
}Message;

// represents ACK Matrix (à changer si besoin @droz)
typedef bool** Ack_matrix;

typedef uint* Ack_list;

// represents an element of the queue, will just be equal to the msg_nr
typedef uint Queue_elem;

// structure representing a queue of messages to be sent. 1 per sender_thread
typedef struct{
  // total number of elements in queue
  size_t no_elem;
  // maximum number of elements in queue allowed
  size_t max_elem;
  // pointer to front of queue
  Queue_elem* front;
  // pointer to back of queue
  Queue_elem* back;
}Msg_queue;

// this structure is just used to associate a thread with its own message queue
typedef struct{
  // the id of the sender_thread
  uint thread_id;
  // the msg queue associated with a thread
  Msg_queue* queue;
}Thread_msg_queue;

// structure representing the list of message queues, 1 for all threads
typedef struct{
  // total number of sender threads
  size_t no_sender;
  Thread_msg_queue* queues;
}Queue_list;


// structure to represent another process and its respective address
// we still have to choose if we will just use a list or a structure for the address list
typedef struct{
  uint process_id;
  // address structure for sending messages over UDP
  struct sockaddr_in* address;
}Address_entry;

// made a typedef for the address list, can be changed, I thought it might be useful
// to create an interface for handling it
typedef Address_entry* Address_Book;

// structure representing all the information a sender_tread will have access to
typedef struct{
  // unique id for sender_thread (might be set as the same as process_id)
  uint thread_id;
  // Address and unique id of the associated process
  Address_entry* process_address;
  // the sender_thread's msg queue
  Thread_msg_queue* queue;
  //pointer to the ack matrix
  Ack_matrix* acks;
}Sender_info;


// structure reprenting all info a receiver thread needs, might be subject to change
// in the case where we add a new thread responsible for sending acks
typedef struct{
  // pointer to the address book of other processes
  Address_Book* addresses;
  // pointer to the list of thread queues
  Queue_list* thread_queues;
  // pointer to ack matrix
  Ack_matrix* ack_matrix;

}Receiver_info;
