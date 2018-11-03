// Fair loss layer


#include "error.h"
#include "structure.h"
#include <stdio.h>
#include <stdlib.h>

// fair loss send
int send_fl(int fd, struct sockaddr_in* address, msg_t* msg) {
    int error;
    // sends a message to the corresponding address pointed by data
    error = sendto(fd,(const char*) msg, sizeof(msg_t),
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

int deliver_fl(receiver_info_t* data, struct sockaddr_in* sender, msg_t* msg){
    
}