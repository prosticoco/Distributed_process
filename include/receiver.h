#pragma once

#include <pthread.h>

#include "data.h"

void *receiver_f(void* params);

int init_receiver(net_data_t* data);

int process_msg(net_data_t* data, msg_t msg);