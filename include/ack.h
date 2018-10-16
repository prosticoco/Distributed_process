#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "structure.h"


/**
 * @brief Initialize an ack table, given the number of processes and their info.
 * 
 * @param data The ack table to initialize.
 * @param num_proc The number of processes.
 * @param proc_list List of process info.
 * @return int 0 in case of success, non-0 otherwise.
 */
int init_acks(ack_data_t* data, size_t num_proc, da_process_t* proc_list);

/**
 * @brief Register ack for given pid and message number. Assumes acks
 *        always arrive in the same order the messages have been broadcast.
 * 
 * @param acks The ack table to update.
 * @param pid The pid of the process who received the ack.
 * @param msg_no The number of the message who was acked.
 * @return int 0 in case of success, non-0 otherwise.
 */
int add_ack(ack_data_t* acks, unsigned int pid, unsigned int msg_no);

/**
 * @brief Test if a given message sent by a given process was acked.
 * 
 * @param acks The ack table to check.
 * @param pid PID of the process for which we want to check.
 * @param msg_no Number of the message we want to check.
 * @return int 1 if message was acked, 0 if not, < 0 in case of an error.   
 */
int read_ack(ack_data_t* acks, unsigned int pid, unsigned int msg_no);

/**
 * @brief Free resources allocated to given ack table.
 * 
 * @param acks The ack table.
 * @return int 0 in case of success, non-0 otherwise.
 */
int free_acks(ack_data_t* acks);
