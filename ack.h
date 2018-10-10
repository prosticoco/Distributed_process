#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "structure.h"

int init_acks(ack_data_t* data, size_t num_proc, da_process_t* proc_list);

int add_ack(ack_data_t* acks,unsigned int pid,unsigned int msg_no);

/**
 * @brief Test if a given message sent by a given process was acked.
 * 
 * @param acks The ack table to check.
 * @param pid PID of the process for which we want to check.
 * @param msg_no Number of the message we want to check.
 * @return int 1 if message was acked, 0 if not, < 0 in case of an error.   
 */
int read_ack(ack_data_t* acks, unsigned int pid, unsigned int msg_no);

int free_acks(ack_data_t* acks);
