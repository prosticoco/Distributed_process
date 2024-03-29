#pragma once

#include <pthread.h>
#include <stdio.h>

#include "addrbook.h"

//put message to queue end value
#define BACK_TO_QUEUE 42

//message id
typedef unsigned int mid_t;

/**
 * @brief URB layer structure 
 * urb table entry is a tuple of bool 
 * to know if message is seen and 
 * if it is delivered
 */
typedef struct {
    unsigned int seen;
    unsigned int delivered;
} urb_entry_t;

/**
 * @brief URB layer structure
 * table of urb entries where 
 * indices represent the urb message id
 */
typedef struct{
    urb_entry_t* values;
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
}urb_table_t;

/**
 * @brief FIFO layer struct for
 * table of messages that are pending
 * to be delivered 
 */
typedef struct{
    unsigned int* bol;
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
} pending_t;

/**
 * @brief FIFO layer struct to
 * know which message from which process
 * has to be delivered next 
 */
typedef struct{
    unsigned int no_process;
    unsigned int* entries;
}next_t;

/**
 * @brief abstract structure
 * to use for ack table at PLINK layer 
 * and at URB layer
 */
typedef struct {
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
    unsigned int* entries;
} uid_table_t;

/**
 * @brief PLINK layer struct
 * table of the acks we received
 * receiver thread writes while
 * sender threads read it so 
 * we use a mutex lock on it
 */
typedef struct {
    pthread_mutex_t table_mutex;
    uid_table_t table;
} ack_table_t;

/**
 * @brief FIFO message wrapper
 * lowest level wrapper that takes 
 * the message seq number and the original
 * sender as uints and passes to the level 
 * underneath
 */
typedef struct{
    unsigned int sequence_num;
    unsigned int original_sender;
} fifo_msg_t;



// message structure for lcb
// the vector clock structure to be sent over the causal broadcast messages
typedef struct vec_clock {
    unsigned int num_proc;
    unsigned int* vector;
    pthread_mutex_t mutex;
} vec_clock_t;

typedef struct lcb_msg {
    vec_clock_t vec_clock;
    unsigned int original_sender;
} lcb_msg_t;

/**
 * @brief URB layer message wrapper
 * for fifo AND lcb since these 2 are 
 * theoreticaly at the same layer height
 * Adds message id and seen id that are
 * derived from current sender process and 
 * fifo/lcb message information
 */
typedef struct{
    lcb_msg_t lcb_msg;
    fifo_msg_t fifo_msg;
    unsigned int seen_id;
    mid_t mid;
} urb_msg_t;

/**
 * @brief PLINK message wrapper
 * wraps URB message
 * ackid id of the corresponding ack
 * for ack table
 * sender is current sender
 * mtype is message type to know 
 * if ack or normal message
 */
typedef struct {
    urb_msg_t urb_msg;
    mid_t mid;
    unsigned int ackid;
    unsigned int sender;
    // is 1 if ack, zero otherwise
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

/**
 * @brief PLINK layer struct
 * to keep track of messages that 
 * have been delivered so it 
 * does not happen again 
 */
typedef struct {
    uid_table_t table;
} pl_delivered_t;

/**
 * @brief URB layer struct
 * just as PLINK ack table
 * to keep track of urb layer acks
 */
typedef struct{
    uid_table_t table;
} urb_ack_table_t;

// Forward declaration to make it work.
struct net_data;
typedef struct {
    size_t idx;
    int socket_fd;
    struct net_data* data;
} sender_thread_arg_t;

/**
 * @brief SENDER THREAD STRUCT
 * thing that sends stuff
 */
typedef struct {
    pthread_t thread;
    sender_thread_arg_t args;
} sender_thread_t;

/**
 * @brief buffer for log
 * so that we dont open and 
 * close log files too often
 */
typedef struct{
    size_t buf_max_size;
    size_t buf_current_size;
    char* buffer;
    FILE* file;
    pthread_mutex_t loglok;
}log_data_t;

// structures related to Localized causal broadcast


// the dependencies structure to be initialized at parsing step
typedef struct dependency_list {
    size_t* pid_list;
    size_t list_len;
} dependency_list_t;

typedef struct dependencies {
    // Lists of dependencies
    dependency_list_t* dep_lists;
    // Number of lists
    size_t num_proc;
} dependencies_t;

typedef struct vec_node {
    vec_clock_t clock;
    struct vec_node* next;
    struct vec_node* previous;
}vec_node_t;

typedef struct vec_list {
    size_t num_elems;
    struct vec_node* head;
    struct vec_node* tail;   
} vec_list_t;
// the pending structure for implementing localized causal broadcast
typedef struct lcb_pending {
    size_t num_proc;
    vec_list_t* lists;
} lcb_pending_t;


/**
 * @brief an equivalent 
 * of a handbook that every process
 * has so that it can have all 
 * the information about itself 
 * easily accessible 
 */
typedef struct net_data {  
    // rlist of process addresses
    addr_book_t* address_book;
    // pid of current process
    size_t self_pid;
    // total number of processes
    size_t num_proc;
    // number of messages to be originally sent by current process
    size_t num_msg;
    // table to keep track of acks received (perfect link)
    ack_table_t* pl_acks;
    // delivered table for perfect link
    pl_delivered_t* pl_delivered;
    // task queue for sender threads
    msg_queue_t* task_q;
    // urb table for urb layer
    urb_table_t* urb_table;
    // list of sender threads
    sender_thread_t* senders;
    // receiver thread
    pthread_t receiver;
    // pending table for fifo layer
    pending_t* pending;
    // next table for fifo layer
    next_t* next;
    // table for ack[m]
    urb_ack_table_t* urbacks;
    // file descriptor for receiver socket
    int receiver_fd;
    // log data pointer
    log_data_t* logdata;
    // log file name
    char log_filename[14];
    // mutex for the receiver
    pthread_mutex_t rec_mutex;
    // Localized Causal Broadcast dependency matrix
    dependencies_t* dependencies;
    // reverse dependency list for pending checking
    dependencies_t* reverse_dependencies;
    // pending set for lcb layer
    lcb_pending_t* lcb_pending;
    // vector clock of current process
    vec_clock_t* vector_clock;
} net_data_t;

