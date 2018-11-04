
#include "mqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// initializes a queue with an arbitrary restriction on the maximum number of elements
int init_queue(msg_queue_t* queue, size_t size) {
    int error = pthread_mutex_init(&(queue->queue_mutex),NULL);
    if(error){
        return ERROR_MUTEX;
    }
    queue->no_elem = 0;
    queue->elems = calloc(size, sizeof(queue_task_t));
    if (queue->elems == NULL) {
        return ERROR_MEMORY;
    }
    queue->qsize = size;
    queue->front = 0;
    queue->back = 0;
    return 0;
}

// returns 1 if queue is empty, zero otherwise
int queue_empty(msg_queue_t* queue) {
    return queue->no_elem == 0;
}

// adds an element at the back of the queue
// returns 0 on success. -1 when queue is full, other errors in other cases
int enqueue(msg_queue_t* queue, queue_task_t* elem) {

    
    // check if the queue is full
    if (queue->no_elem == queue->qsize) {
        return ERROR_QUEUE;
    }
    // check valid pointers
    if (queue->elems == NULL) {
        return ERROR_MEMORY;
    }
    // insert element at the back of the queue
    queue->elems[queue->back] = *elem;
    // increase number of elements
    queue->no_elem += 1;
    // update end of queue pointer
    queue->back = (queue->back + 1) % queue->qsize;
    return 0;
}

// pops the next element of the queue
int dequeue(msg_queue_t* queue, queue_task_t* elem) {
    // if queue is empty
    if (queue_empty(queue)) {
        return ERROR_QUEUE;
    }
    // update value pointed by elem argument
    *elem = queue->elems[queue->front];
    // decrease number of elements
    queue->no_elem -= 1;
    // update front of queue pointer
    queue->front = (queue->front + 1) % queue->qsize;
    return 0;
}

// use at the end to free the memory used by the queue
int free_queue(msg_queue_t* queue) {
    free(queue->elems);
    queue->elems = NULL;
    return 0;
}
