#include <stdint.h>

struct da_process {
    // Unique process identifier for our program
    int uid;
    // 4 bytes, 1 byte per element of the address
    uint32_t ipv4_addr;
    // Port number on which the process listens for incoming messages
    uint16_t port_num;
};