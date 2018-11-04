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


// fair loss send


int send_fl(int fd, struct sockaddr_in* address, fl_msg_t* msg) {
    int error;
    // sends a message to the corresponding address pointed by data
    error = sendto(fd,(const char*) msg, sizeof(fl_msg_t),
                    MSG_DONTWAIT,
                    (const struct sockaddr *) address,
                    sizeof(struct sockaddr_in));
    // handles any error due to sendto
    if (error < 0) {
        return ERROR_SEND;
    }
    return 0;
}

//fair loss receive/deliver

int deliver_fl(struct sockaddr_in* sender, msg_t* msg){
    int error;
    error = deliver_pl(struct sockaddr_in* sender, msg_t* msg);
    return error;
}