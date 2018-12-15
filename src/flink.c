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
    error = send_message(data,socket_fd,dest_pid,msg);
    if(error){
        printf("Error sending message in send_fl \n");
        return error;
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
int deliver_fl(net_data_t* data, msg_t msg){
    int error = deliver_pl(data, msg);
    return error;
}