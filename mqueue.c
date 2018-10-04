#include "structure.h"
#include "mqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

// initializes a queue with an arbitrary restriction on the maximum number of elements
int init_queue(Msg_queue* queue, size_t max_elem){
  queue->no_elem = 0;
  queue->max_elem = max_elem;
  queue->front = NULL;
  queue->back = NULL;
  return 0;
}

// adds an element at the back of the queue
// returns 0 on success. -1 when queue is full, other errors in other cases
int enqueue(Msg_queue* queue, Queue_elem elem){
  if(queue->no_elem == queue->max_elem){
    return -1;
  }
  queue->front = realloc((queue->no_elem + 1)*(sizeof(Queue_elem)));
  if(queue->front == NULL){
    return ERROR_MEMORY;
  }
  queue->no_elem ++;
  if(queue->no_elem == 1){
    queue->back = queue->front;
  }else{
    queue->back = &(queue->front[no_elem-1]);
  }
  *(queue->back) = elem;
  return 0;
}

// adds an element at the front of the queue

// pops the next element of the queue
int dequeue(Msg_queue* queue, Queue_elem* elem);

// use at the end to free the memory used by the queue
int free_queue(Msg_queue* queue);
