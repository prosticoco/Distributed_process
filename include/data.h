#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include "addrbook.h"
#include "plink.h"


typedef struct{
    // unique identifier
    mid_t mid;
    // if 0 then it is not an ack
    unsigned int ack;
}pl_msg_t;

typedef struct{
    unsigned int original_sender;
    unsigned int sender;
    unsigned int no_seen;
    pl_msg_t pl_msg;
}urb_msg_t;

typedef struct{
    unsigned int sequence_num;
}fifo_msg_t;


typedef struct{
    fifo_msg_t fifo_msg;
}msg_t;

typedef struct{  
    struct sockaddr_in* address;
    addr_book_t address_book;
    int fd;
}net_data_t
