#pragma once

#include <pthread.h>

#include "data.h"

/**
 * @brief Function call back for the receiver thread
 * The unique receiver thread will handle reception of all messages
 * @param params will be cast into a net_data_t* pointer
 * @return void* never returns, some people theorize it might return after the next ice age. 
 */
void *receiver_f(void* params);

/**
 * @brief inits the sockets and other components needed for the receiver thread
 * 
 * @param data the data of the process
 * @return 0 upon success, negative error code otherwise
 */
int init_receiver(net_data_t* data);

/**
 * @brief function called after reception of a message
 * to process it
 * 
 * @param data the process data
 * @param msg the message received
 * @return int 0 upon success, negative error code otherwise.
 */
int process_msg(net_data_t* data, msg_t msg);

/**
 * @brief called at the end of the program to kill the receiver thread
 * safely
 * 
 * @param data the process data 
 * @return int 0 upon success, negative error code otherwise
 */
int terminate_receiver(net_data_t* data);