#pragma once

#include "data.h"
//best effort broadcast

//when asked to broadcast message m, need to create message for every process in addr book 
//and add this task to the queue

int beb_send(net_data_t* data, urb_msg_t* msg);

int beb_deliver(net_data_t* data, msg_t* msg);