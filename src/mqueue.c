#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mqueue.h"
#include "error.h"

#define EMPTY_QUEUE -1


// initializes a queue with an arbitrary restriction on the maximum number of elements
int init_queue(msg_queue_t* queue, size_t size) {
    fflush(stdout);
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

int realloc_queue(msg_queue_t* queue){
    // copy the queue
    msg_queue_t old_queue = *queue;
    // create a new memory page for bigger queue
    queue->elems = calloc(old_queue.qsize*2,sizeof(queue_task_t));
    if(queue->elems == NULL){
        return ERROR_MEMORY;
    }
    int index = old_queue.front;
    for(int i = 0 ; i < old_queue.no_elem; i++){
        queue->elems[i] = old_queue.elems[index];
        index = (index + 1) % (old_queue.qsize);
    }
    // update the control values
    queue->front = 0;
    queue->back = old_queue.no_elem;
    queue->qsize = old_queue.qsize*2;
    queue->no_elem = old_queue.no_elem;
    // free memory of old queue
    free(old_queue.elems);
    return 0;
}

// adds an element at the back of the queue
// returns 0 on success. -1 when queue is full, other errors in other cases
int enqueue(msg_queue_t* queue, queue_task_t* elem) {

    pthread_mutex_lock(&(queue->queue_mutex));
    // check if the queue is full
    int error;
    if (queue->no_elem == queue->qsize) {
        error = realloc_queue(queue);
        if(error){
            return error;
        }
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
    pthread_mutex_unlock(&(queue->queue_mutex));
    return 0;
}

// pops the next element of the queue
int dequeue(msg_queue_t* queue, queue_task_t* elem) {

    pthread_mutex_lock(&(queue->queue_mutex));
    // if queue is empty
    if (queue_empty(queue)) {
        return EMPTY_QUEUE;
    }
    // update value pointed by elem argument
    *elem = queue->elems[queue->front];
    // decrease number of elements
    queue->no_elem -= 1;
    // update front of queue pointer
    queue->front = (queue->front + 1) % queue->qsize;

    pthread_mutex_unlock(&(queue->queue_mutex));
    return 0;
}

// use at the end to free the memory used by the queue
int free_queue(msg_queue_t* queue) {
    free(queue->elems);
    queue->elems = NULL;
    return 0;
}
