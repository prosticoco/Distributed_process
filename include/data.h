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
}fifo_msg_t;





typedef struct{  
    addr_book_t* address;
    addr_book_t address_book;
    int fd;

    ack_table_t* pl_acks;
    msg_queue_t* task_q;
}net_data_t;
