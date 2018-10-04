
#include "mqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// initializes a queue with an arbitrary restriction on the maximum number of elements
int init_queue(Msg_queue* queue, size_t max_elem){
  queue->no_elem = 0;
  queue->max_elem = max_elem;
  queue->front = NULL;
  queue->back = NULL;
  return 0;
}

// returns 1 if queue is empty, zero otherwise
int queue_empty(Msg_queue* queue){
  return(queue->no_elem == 0);
}

// adds an element at the back of the queue
// returns 0 on success. -1 when queue is full, other errors in other cases
int enqueue(Msg_queue* queue, Queue_elem elem){
  if(queue->no_elem == queue->max_elem){
    return ERROR_QUEUE;
  }
  queue->front = realloc(queue->front,(queue->no_elem + 1)*(sizeof(Queue_elem)));
  if(queue->front == NULL){
    return ERROR_MEMORY;
  }
  queue->no_elem ++;
  if(queue->no_elem == 1){
    queue->back = queue->front;
  }else{
    queue->back = &(queue->front[queue->no_elem-1]);
  }
  *(queue->back) = elem;
  return 0;
}

// pops the next element of the queue
int dequeue(Msg_queue* queue, Queue_elem* elem){
  // if queue is empty
  if(queue_empty(queue)){
    return ERROR_QUEUE;
  }
  // update the element
  *elem = *(queue->front);
  Queue_elem* new_ptr = NULL;
  // allocate a new pointer for new array equal to the new size
  new_ptr = realloc(new_ptr,(queue->no_elem -1)*(sizeof(Queue_elem)));
  if(new_ptr == NULL){
    return ERROR_MEMORY;
  }
  // decrease number of elements after successfull realloc
  queue->no_elem--;
  if(queue_empty(queue)){
    // if the queue is empty now, we free the old pointer and set other pointers to null
    free(queue->front);
    queue->back = NULL;
    queue->front = NULL;
  }else{
    // if queue is nonempty we move the remaining elements to the new_ptr and then free the old pointer
    new_ptr = memmove(new_ptr,&(queue->front[1]),(queue->no_elem)*(sizeof(Queue_elem)));
    free(queue->front);
    // update the queues pointer
    queue->front = new_ptr;
    queue->back = &(queue->front[queue->no_elem-1]);
  }
  return 0;
}

// use at the end to free the memory used by the queue
int free_queue(Msg_queue* queue){
  free(queue->front);
  queue->front = NULL;
  queue->back = NULL;
  return 0;
}
