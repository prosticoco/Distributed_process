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

#include "layers.h"
#include "plink.h"
#include "data.h"
#include "error.h"

/**
 * @brief Send message to destination pid.
 * 
 * @param data Pointer to the global data.
 * @param thread_idx Index of the thread calling the function.
 * @param dest_pid Pid of the destination process.
 * @param msg Msg to send.
 * @return int 0 in case of success, non-0 otherwise.
 */
int send_fl(net_data_t* data, int socket_fd, unsigned int dest_pid, msg_t msg) {
    int error;
    struct sockaddr_in address;
    error = get_addr(data->address_book, dest_pid, &address);
    if(error < 0){
        return error;
    }
    // sends a message to the corresponding address pointed by data
    error = sendto(socket_fd,(const char*) &msg, sizeof(msg_t),
                    MSG_DONTWAIT,
                    (const struct sockaddr *) &address,
                    sizeof(struct sockaddr_in));
    // handles any error due to sendto
    if (error < 0) {
        return ERROR_SEND;
    }
    return 0;
}

/**
 * @brief fair loss deliver
 * 
 * @param data all data we need
 * @param thread_idx
 * @param msg message we received
 * @return int 
 */
int deliver_fl(net_data_t* data, size_t thread_idx, msg_t msg){
    int error = deliver_pl(data, thread_idx, msg);
    return error;
}