#pragma once

#include "data.h"

//Uniform reliable broadcast


//list of uids that are delivered in urb
//AND list of uids that are pending to be delivered 
//i.e. are waiting for enough processes to be in candeliver
//but these 2 are same thing just different name and purpose but same structure
// table_uid_t delivered_urb, table_uid_t pending
int is_delivered_urb(urb_table_t* table, unsigned int seen_id);

int add_delivered_urb(urb_table_t* table,unsigned int seen_id);

int is_seen_urb(urb_table_t* table,unsigned int seen_id);

int add_seen_urb(urb_table_t* table, unsigned int seen_id);

int free_urb_table(urb_table_t* table);


int init_urb_table(urb_table_t* table, unsigned int no_msgs, unsigned int no_process);

int init_ack_urb(urb_ack_table_t* delivered,unsigned int no_entries, unsigned int no_process);

int set_ack_urb(urb_ack_table_t* delivered,mid_t mid);

int free_ack_urb(urb_ack_table_t* delivered);

int can_deliver(urb_ack_table_t* delivered,unsigned int seen_id);



