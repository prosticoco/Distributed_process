#pragma once

#include "addrbook.h"
#include "mqueue.h"

typedef struct {
    struct sockaddr_in* address;
    int fd;
} fl_data_t;

typedef struct {

} pl_data_t;

typedef struct {
    addr_book_t* address_book;
} beb_data_t;

typedef struct{
    addr_book_t* address_book;
}urb_data_t;

typedef struct{
    unsigned int sequence_num;
    unsigned int original_sender;
}fifo_msg_t;

/** every urb message needs the original message that is in beb_msg
 *  and the pid of the original sender and the current sender
 */
typedef struct{
    fifo_msg_t fifo_msg;
    unsigned int seen_id;
    unsigned int no_seen;
} urb_msg_t;


typedef struct {
    urb_msg_t urb_msg;
    mid_t mid;
    unsigned int sender;
    unsigned int mtype;
} msg_t;



typedef struct{  
    addr_book_t* address_book;
    unsigned int self_pid;
    int fd;
    ack_table_t* pl_acks;
    msg_queue_t* task_q;
}net_data_t;
