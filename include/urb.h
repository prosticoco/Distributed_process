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
}urb_table_t;

int is_delivered_urb(urb_table_t* table, unsigned int seen_id);

int add_delivered_urb(urb_table_t* table,unsigned int seen_id);

int is_seen_urb(urb_table_t* table,unsigned int seen_id);

int add_seen_urb(urb_table_t* table, unsigned int seen_id);

int free_urb_table(urb_table_t* table);


int init_urb_table(urb_table_t* table, unsigned int no_msgs, unsigned int no_process);



