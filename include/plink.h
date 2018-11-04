#pragma once

#include <pthread.h>

#include "data.h"
#include "flink.h"
#include "beb.h"
#include "urb.h"



typedef struct{
    unsigned int no_msgs;
    unsigned int no_process;
    unsigned int total_entries;
    unsigned int* entries;
}uid_table_t;

typedef struct{
    pthread_mutex_t table_mutex;
    uid_table_t table;
}ack_table_t;

typedef struct{
    uid_table_t table;
}pl_delivered_t;


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

/**
 * @brief perfect link send
 * 
 * @param pid pid of process we send to
 * @param data all data we need
 * @param msg msg to send
 * @return int 
 */
int pl_send(unsigned int pid,net_data_t* data, msg_t msg);

/**
 * @brief perfect link deliver
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int pl_deliver(net_data_t* data, msg_t msg);