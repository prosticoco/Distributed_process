#include "mqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

  msg_queue_t tqueue;
  int err = init_queue(&tqueue,10);
  if(err < 0){
    return 0;
  }
  queue_elem_t elem1 =(queue_elem_t) 1;
  queue_elem_t elem2 =(queue_elem_t) 2;
  printf("element 1 = %d , element 2 = %d\n",elem1,elem2);
  err = enqueue(&tqueue,&elem2);
  err = enqueue(&tqueue,&elem1);
  err = dequeue(&tqueue,&elem1);
  err = dequeue(&tqueue,&elem2);
  err = free_queue(&tqueue);
  printf("element 1 = %d , element 2 = %d\n",elem1,elem2);
  return 0;
}
