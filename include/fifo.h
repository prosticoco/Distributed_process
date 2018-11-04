#pragma once

#include "data.h"



int add_pending(pending_t* pending,unsigned int seen_id,unsigned int value);

int get_pending(pending_t* pending, unsigned int seen_id);

int free_pending(pending_t* pending);

int init_pending(pending_t* pending, unsigned int no_msgs, unsigned int no_process);

int realloc_pending(pending_t* pending);