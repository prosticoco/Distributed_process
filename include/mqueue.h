#pragma once

#include <stdlib.h>

#include "data.h"

// initializes a queue with an arbitrary restriction on the maximum number of elements
// should always return 0 
int init_queue(msg_queue_t* queue, size_t size);

// returns 1 if queue is empty, zero otherwise
int queue_empty(msg_queue_t* queue);

// adds an element at the back of the queue
int enqueue(msg_queue_t* queue, queue_task_t* elem);

// pops the next element of the queue
int dequeue(msg_queue_t* queue, queue_task_t* elem);

// use at the end to free the memory used by the queue
int free_queue(msg_queue_t* queue);


/**
 * @brief reallocs the queue size when it gets full
 * 
 * @param queue 
 * @return int 
 */
int realloc_queue(msg_queue_t* queue);

/**
 * @brief frees the queue memory and fields 
 * (to be called at the end of the programm)
 * 
 * @param queue 
 * @return int 
 */
int free_queue(msg_queue_t* queue);

/**
 * @brief frees the pointers refering to vector clocks in msgs
 * (to be called at the end of the program)
 * 
 * @param queue 
 * @return int 
 */
int free_queue_and_vectors(msg_queue_t* queue); 
