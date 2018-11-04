#pragma once

#include <pthread.h>

#include "addrbook.h"


typedef unsigned int mid_t;



typedef struct {
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
    unsigned int* entries;
} uid_table_t;

typedef struct {
    pthread_mutex_t table_mutex;
    uid_table_t table;
} ack_table_t;

typedef struct {
    struct sockaddr_in* address;
    int fd;
} fl_data_t;

typedef struct {

} pl_data_t;

typedef struct {
    addr_book_t* address_book;
} beb_data_t;

typedef struct{
    unsigned int sequence_num;
    unsigned int original_sender;
} fifo_msg_t;

/** every urb message needs the original message that is in beb_msg
 *  and the pid of the original sender and the current sender
 */
typedef struct{
    fifo_msg_t fifo_msg;
    unsigned int seen_id;
    unsigned int no_seen;
} urb_msg_t;


typedef struct {
    urb_msg_t urb_msg;
    mid_t mid;
    unsigned int sender;
    unsigned int mtype;
} msg_t;

// represents an element of the queue, will just be equal to the msg_nr
typedef struct {
    unsigned int pid_dest;
    // msg struct
    msg_t msg;
} queue_task_t;

// structure representing a queue of messages to be sent. 1 per sender_thread
typedef struct {
    // the number of elements in the queue
    unsigned int no_elem;
    // the size of the queue
    unsigned int qsize;
    // index corresponding to the front of the queue
    unsigned int front;
    // index corresponding to the back of the queue
    unsigned int back;
    // pointer to the elements of the queue
    queue_task_t* elems;

    pthread_mutex_t queue_mutex;

} msg_queue_t;

typedef struct {  
    addr_book_t* address_book;
    size_t self_pid;
    size_t num_proc;
    size_t num_msg;
    int fd;
    ack_table_t* pl_acks;
    msg_queue_t* task_q;
} net_data_t;
