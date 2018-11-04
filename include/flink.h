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
#include "urb.h"
#include "data.h"
#include "error.h"


// Structures for Layer 0

typedef unsigned int mid_t;

typedef struct {
    urb_msg_t* urb_msg;
    mid_t mid;
    unsigned int mtype;
} msg_t;

// fair loss send
int send_fl(net_data_t* data, unsigned int dest_pid, msg_t* msg);

//fair loss receive/deliver
int deliver_fl(net_data_t* data, msg_t* msg);