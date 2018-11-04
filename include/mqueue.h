#pragma once

#include <stdlib.h>

#include "data.h"

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
