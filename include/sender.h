#pragma once

#include <pthread.h>

#include "data.h"

/**
 * @brief Callback function for sender threads.
 * 
 * @param params Function parameters.
 */
void *sender_f(void* params);

int init_senders_and_sockets(net_data_t* data, size_t num_senders);