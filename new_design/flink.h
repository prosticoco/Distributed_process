// Fair loss layer
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>

#include "plink.h"

#include "error.h"


// Structures for Layer 0
#define MAX_MESSAGE_LENGTH 256

typedef struct {
    unsigned int msg_type;
} fl_msg_t;

// fair loss send
int send_fl(int fd, struct sockaddr_in* address, fl_msg_t* msg)

//fair loss receive/deliver
int deliver_fl(struct sockaddr_in* sender, msg_t* msg)