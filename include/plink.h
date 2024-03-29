#pragma once

#include <pthread.h>

#include "data.h"





// Interface for table handling

int init_table_uid(uid_table_t* table,unsigned int no_entries, unsigned int no_process);

int table_read_entry(uid_table_t* table,mid_t mid);

int table_write_entry(uid_table_t* table,mid_t mid,unsigned int value);

int free_table_uid(uid_table_t* table);

// interface for ack table

int init_ack_table(ack_table_t* acks, unsigned int no_entries, unsigned int no_process);

int is_ack(ack_table_t* acks,mid_t mid);

int set_ack(ack_table_t* acks,mid_t mid);

int free_ack_table(ack_table_t * acks);

// Interface for delivered table

int init_deliver_pl(pl_delivered_t* delivered,unsigned int no_entries, unsigned int no_process);

int is_delivered(pl_delivered_t* delivered,mid_t mid);

int set_delivered(pl_delivered_t* delivered,mid_t mid);

int free_delivered(pl_delivered_t* delivered);
