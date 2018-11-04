#ifndef MQUEUE_H
#define MQUEUE_H


#include "error.h"
#include <stdlib.h>

// represents an element of the queue, will just be equal to the msg_nr
typedef struct{
    unsigned int pid_dest;
    // msg struct
}queue_task_t;

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

// initializes a queue with an arbitrary restriction on the maximum number of elements
// should always return 0 lolol
int init_queue(msg_queue_t* queue, size_t size);

// returns 1 if queue is empty, zero otherwise
int queue_empty(msg_queue_t* queue);

// adds an element at the back of the queue
int enqueue(msg_queue_t* queue, queue_task_t* elem);

// pops the next element of the queue
int dequeue(msg_queue_t* queue, queue_task_t* elem);

// use at the end to free the memory used by the queue
int free_queue(msg_queue_t* queue);

#endif
