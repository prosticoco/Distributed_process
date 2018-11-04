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
    unsigned int sender_pid;
} msg_t;

/**
 * @brief fair loss send
 * 
 * @param data ptr to all data we need
 * @param dest_pid pid of process we want to send msg
 * @param msg msg we want to send
 * @return int 
 */
int send_fl(net_data_t* data, unsigned int dest_pid, msg_t msg);

/**
 * @brief fair loss deliver
 * 
 * @param data all data we need
 * @param msg message we received
 * @return int 
 */
int deliver_fl(net_data_t* data, msg_t msg);