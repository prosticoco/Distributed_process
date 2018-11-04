#pragma once

#include <pthread.h>

#include "data.h"

/**
 * @brief Callback function for sender threads.
 * 
 * @param params Function parameters.
 */
void *sender_f(void* params);

/**
 * @brief Initialize sender threads and their sockets.
 * 
 * @param data Global data structure containing the array of threads and the sockets.
 * @param num_senders Number of threads and sockets (equal).
 * @return int 0 if success, non-0 otherwise.
 */
int init_senders_and_sockets(net_data_t* data, size_t num_senders);