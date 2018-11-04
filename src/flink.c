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
#include "flink.h"

#include "data.h"
#include "error.h"


// fair loss send
int send_fl(net_data_t* data, msg_t* msg) {
    int error;
    // sends a message to the corresponding address pointed by data
    error = sendto(data->fd,(const char*) msg, sizeof(msg_t),
                    MSG_DONTWAIT,
                    (const struct sockaddr*) data->address,
                    sizeof(struct sockaddr_in));
    // handles any error due to sendto
    if (error < 0) {
        return ERROR_SEND;
    }
    return 0;
}

//fair loss receive/deliver
int deliver_fl(net_data_t* data, msg_t* msg){
    int error = deliver_pl(data, msg);
    return error;
}