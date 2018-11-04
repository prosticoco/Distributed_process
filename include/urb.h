#pragma once

#include "error.h"
#include "data.h"

//Uniform reliable broadcast


/** every urb message needs the original message that is in beb_msg
 *  and the pid of the original sender and the current sender
 */
typedef struct{
    size_t orig_sender_pid;
    size_t sender_pid;
    beb_msg_t* beb_msg;
}urb_msg_t;

//list of uids that are delivered in urb
//AND list of uids that are pending to be delivered 
//i.e. are waiting for enough processes to be in candeliver
//but these 2 are same thing just different name and purpose but same structure
// table_uid_t delivered_urb, table_uid_t pending

typedef struct{
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
    unsigned int* entries;
} table_uid_t;

//for all m we need to make an ack table to know which process has seen the message
typedef struct{
    //JE SAIS POOOOOOOOOO
} ack_table_t;


int urb_send(urb_data_t* data, urb_msg_t* msg);

int urb_deliver(urb_data_t* data, urb_msg_t* msg);

bool candeliver(urb_msg_t* msg);

int init_table_uid(uid_table_t* table,unsigned int no_entries, unsigned int no_process);

int table_read_entry(uid_table_t* table,urb_msg_t* msg);

int table_write_entry(uid_table_t* table,urb_msg_t* msg,unsigned int value);

int free_table_uid(uid_table_t* table);