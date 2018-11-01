#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ack.h"
#include "sender.h"
#include "error.h"
#include "structure.h"

int test_send1(sender_info_t* data) {
    int error;
    msg_t msg;
    msg.msg_nr = 1;
    msg.msg_type = MSG_NO;
    msg.src_id = 1;
    error = send_pl(data, &msg);
    printf("SENDER THREAD : message number one was successfully sent\n");
    if (error) {
        return error;
    }
    msg.msg_nr = 2;
    error = send_pl(data, &msg);
    if (error) {
        return error;
    }
    return 0;

}

// callback function for sender thread, ie main function for thread
void *sender_f(void * params) {
    int error;
    sender_info_t* data_s = (sender_info_t*) params;
    
    printf("sender thread for proc no : %d \n", data_s->process_address->process_id);
    printf("Sender thread : I am happy to announce that I will send some messages to port number %d\n", data_s->process_address->address.sin_port);
    // WAIT TO START the following code temporarily locks a mutex (mutex gets unlocked and locked again while cond_wait)
    error = pthread_mutex_lock(data_s->start_m);
    if (error) {
        fprintf(stderr, "ERROR pthread_mutex_lock() error code %d\n", error);
        exit(ERROR_MUTEX);
    }
    error = pthread_cond_wait(data_s->start, data_s->start_m);
    if (error) {
        fprintf(stderr, "ERROR pthread_cond_wait() error code %d\n", error);
        exit(ERROR_CONDITION);
    }
    error = pthread_mutex_unlock(data_s->start_m);
    if (error) {
        fprintf(stderr,"ERROR pthread_mutex_unlock() error code %d\n",error);
        exit(ERROR_MUTEX);
    }
    printf("I can now send messages :)\n");
    // FINISH WAIT TO START, the thread will now start sending its messages
    if (data_s->nodeid == 1) {
        test_send1(data_s);
    }
    while (1) {
        // send message 
    }
    return 0;
}



//int test_send2(sender_info_t* data);


int init_senders(receiver_info_t* data) {
    unsigned int no_nodes = data->no_nodes;
    // allocate array of sender_info_t 
    sender_info_t* sender_array = calloc(no_nodes, sizeof(sender_info_t));
    memset(sender_array, 0, no_nodes * sizeof(sender_info_t));
    // add the pointer in the data structure
    data->sender_infos = sender_array;
    if (sender_array == NULL) {
        fprintf(stderr,"ERROR calloc() in init_senders()");
        return ERROR_MEMORY;
    }
    // copy locally/temporarily the address book for faster access
    addr_book_t* book = &(data->addresses);
    // copy temporaily list of counters 
    ack_data_t* counters = &(data->acklist);
    int error;
    sender_info_t* curr_s = NULL;
    // iterate over each sender_info_t structure to initialize all the fields according 
    // to the data
    for (int i = 0; i < no_nodes; i++) {
        // the main node id 
        curr_s = &(sender_array[i]);
        curr_s->nodeid = data->nodeid;
        // set all the other fields accordingly
        curr_s->process_address = &(book->listaddr[i]);
        curr_s->queue = NULL;
        curr_s->acklist = counters;
        // copy the address of the condition mutex to start sending messages
        curr_s->start = data->start;
        curr_s->start_m = data->start_m;
        // when everything is done initialize the socket (so that the filedescriptor of the sender gets updated too)
        error = init_socket_sender(curr_s);
        if (error) {
            return error;
        }
        // everything is set up so we can now spawn the corresponding thread
        error = pthread_create(&data->senders[i], NULL, sender_f, (void*) curr_s);
        if (error) {
            return ERROR_THREAD;
        }
    }
    return 0;
}

int init_socket_sender(sender_info_t* data) {
    // open a new socket
    int desc = socket(AF_INET,SOCK_DGRAM,0);
    // check if any error
    if (desc < 0) {
        return ERROR_SOCKET;
    }
    // update the thread's file descriptor
    data->fd = desc;
    return 0;
}

int send_pl(sender_info_t* data,msg_t* msg) {
    int error = 0;
    int i = 0;
    while (!read_ack(data->acklist, data->process_address->process_id, msg->msg_nr)) {
        i+= 1;
        printf("look at me I am in a while \n");
        error = send_fl(data->fd, &(data->process_address->address), msg);
        printf("just sent a message via fair loss link, what a story mark\n");
        if (error < 0){
            return ERROR_SEND;
        }
        usleep(2000);
    }
    printf("exiting perfect link, number of iterations in while loop = %d\n",i);
    return 0;
}


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

//simple best effort broadcast uses pl_send
int broadcast_beb(msg_t* msg, addr_book_t* book);


//unified reliable broadcast that uses beb
int broadcast_urb(msg_t* msg, addr_book_t* book) {

    /**pending.add(self,msg->msg_nr)
     * broadcast_beb(msg);
     */
    return 0;
}


int terminate_senders(receiver_info_t* data) {
    int error;
    // iterate over all file descriptors and close the sockets
    for(int i = 0; i < data->no_nodes; i++) {
        error = close(data->sender_infos[i].fd);
        if (error < 0) {
            return ERROR_CLOSE;
        } 
    }
    free(data->sender_infos);
    return 0;
}
