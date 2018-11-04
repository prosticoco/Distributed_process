#pragma once

#include "data.h"

//Uniform reliable broadcast


//list of uids that are delivered in urb
//AND list of uids that are pending to be delivered 
//i.e. are waiting for enough processes to be in candeliver
//but these 2 are same thing just different name and purpose but same structure
// table_uid_t delivered_urb, table_uid_t pending

typedef struct {
    unsigned int seen;
    unsigned int delivered;
} urb_entry_t;

typedef struct{
    urb_entry_t* values;
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
} urb_table_t;

int init_urb_table(urb_table_t* table, unsigned int no_msgs, unsigned int no_process);


// Main Algorithm methods

int urb_send(net_data_t* data, urb_msg_t* msg);

int urb_deliver(net_data_t* data, urb_msg_t* msg);

int candeliver(net_data_t* msg);

