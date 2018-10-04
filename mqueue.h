#ifndef 


#include "structure.h"
#include "error.h"
#include <stdlib.h>


// initializes a queue with an arbitrary restriction on the maximum number of elements
// should always return 0 lolol
int init_queue(Msg_queue* queue, size_t max_elem);

// returns 1 if queue is empty, zero otherwise
int queue_empty(Msg_queue* queue);

// adds an element at the back of the queue
int enqueue(Msg_queue* queue, Queue_elem elem);

// pops the next element of the queue
int dequeue(Msg_queue* queue, Queue_elem* elem);

// use at the end to free the memory used by the queue
int free_queue(Msg_queue* queue);
