#ifndef DA_PROC_H_INCLUDED
#define DA_PROC_H_INCLUDED

#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct da_process {
    // Unique process identifier for our program
    int uid;
    // 4 bytes, 1 byte per element of the address
    struct in_addr ipv4_addr;
    // Port number on which the process listens for incoming messages
    in_port_t port_num;
} da_process_t;

#endif