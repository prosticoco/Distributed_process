#pragma once

// Fair loss layer
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>

#include "plink.h"

#include "data.h"
#include "error.h"


// Structures for Layer 0
typedef struct {
    unsigned int msg_type;
} fl_msg_t;

// fair loss send
int send_fl(net_data_t* data, fl_msg_t* msg);

//fair loss receive/deliver
int deliver_fl(net_data_t* data, msg_t* msg);