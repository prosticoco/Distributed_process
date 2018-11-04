#pragma once

#include "addrbook.h"

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

}fifo_data_t;




typedef struct{
    fl_data_t* fldata;
    pl_data_t* pldata;
    beb_data_t* bebdata;
    urb_data_t* urbdata;
    fifo_data_t* fifodata;    
}net_data_t;


