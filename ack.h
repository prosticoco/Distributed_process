#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "structure.h"

int init_acks(ack_data_t* acks,size_t num_msg,size_t num_proc,da_process_t* proc_list);

int add_ack(ack_data_t* acks,unsigned int pid,unsigned int msg_no);

int read_ack(ack_data_t* acks,unsigned int pid, unsigned int msg_no);

int free_acks(ack_data_t* acks);
