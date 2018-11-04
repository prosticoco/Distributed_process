#pragma once

#include "data.h"



int add_pending(pending_t* pending,unsigned int seen_id,unsigned int value);

int get_pending(pending_t* pending, unsigned int seen_id);

int free_pending(pending_t* pending);

int init_pending(pending_t* pending, unsigned int no_msgs, unsigned int no_process);

int realloc_pending(pending_t* pending);

int init_next(next_t* next,unsigned int no_process);

int get_next(next_t* next, unsigned int pid);

int incr_next(next_t* next,unsigned int pid);

int free_next(next_t* next);

int init_fifo_structures(net_data_t* data,unsigned int no_process, unsigned int no_msgs);

int free_fifo_structures(net_data_t* data);