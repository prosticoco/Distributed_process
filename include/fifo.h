#pragma once

#include "data.h"



int add_pending(pending_t* pending,unsigned int seen_id);

int get_pending(pending_t* pending, unsigned int seen_id);

int free_pending(pending_t* pending);

int init_pending(pending_t* pending, unsigned int no_msgs, unsigned int no_process);

