#pragma once

#include "data.h"

int init_log_data(log_data_t* logdata,size_t buf_max_size,char* filename);

int write_log(log_data_t* logdata, char* line,size_t size);

int write_to_file(log_data_t* logdata);

int free_log_data(log_data_t* logdata);

// method will write that a message was delivered to the log-buffer
// method is defined in fifo.c
int log_deliver(net_data_t* data,fifo_msg_t msg);