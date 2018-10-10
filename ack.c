#include "ack.h"
#include "error.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>



// lock to use on matrix
static pthread_rwlock_t ack_lock;
static pthread_rwlockattr_t attribute;
static int lock_in_use = 0;




// only call once initializes acks structure and initializes a reader/writer lock
int init_acks(ack_data_t* data,size_t num_proc,da_process_t* proc_list){
  int error;
  if(lock_in_use){
    return ERROR_ACK_LOCK;
  }
  error = pthread_rwlock_init(&ack_lock,&attribute);
  if(error){
    return ERROR_LOCK;
  }
  error = pthread_rwlockattr_setkind_np(&attribute,PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
  if(error){
    return ERROR_LOCK;
  }
  lock_in_use = 1;
  data->acks = calloc(num_proc,sizeof(ack_counter_t));
  if(data->acks == NULL){
    return ERROR_MEMORY;
  }
  for(int i = 0 ; i < num_proc; i++){
    data->acks[i].counter = 0;
    data->acks[i].pid = proc_list[i].uid;
  }
  return 0;
}

int add_ack(ack_data_t* acks,unsigned int pid,unsigned int msg_no){
  
  int error;
}

int read_ack(ack_data_t* acks,unsigned int pid, unsigned int msg_no);

int free_acks(ack_data_t* acks);
