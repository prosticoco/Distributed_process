#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "data.h"
#include "error.h"
#include "layers.h"
#include "mqueue.h"
#include "sender.h"

#define EMPTY_QUEUE_WAIT 10000


/**
 * @brief Thread termination callback. Releases any held lock and exits.
 * 
 * @param params 
 * @return void* 
 */
static void *sender_terminate(void* params) {
    // WARNING: can only call 'safe' function, check man 7 signal
}

/**
 * @brief Callback function for sender threads.
 * 
 * @param params Function parameters.
 */
static void *sender_f(void* params) {
    sender_thread_arg_t* args = params;
    net_data_t* data = args->data;
    msg_queue_t* msg_queue = data->task_q;

    // Set own cancelability state to asynchronous to allow for
    // (hopefully) immediate cancellation upon pthread_cancel.
    int dump;
    if (!pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump)) {
        pthread_exit((void *) ERROR_THREAD);
    }

    while (1) {
        queue_task_t msg_task;
        while (dequeue(msg_queue, &msg_task)) {
            usleep(EMPTY_QUEUE_WAIT);
        }

        int res = send_pl(msg_task.pid_dest, args->socket_fd, data, msg_task.msg);
        if (res) {
            pthread_exit((void *) ERROR_SEND);
        }
    }

    return NULL;
}


static int setup_sockets(net_data_t* data, size_t num_senders) {
    // Allocate senders.
    data->senders = calloc(num_senders, sizeof(sender_thread_t));
    if (!data->senders) {
        return ERROR_MEMORY;
    }
    // Set their pointer to the data pointer and set their index.
    for (size_t i = 0; i < num_senders; ++i) {
        data->senders[i].args.idx = i;
        data->senders[i].args.data = data;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            free(data->senders);
            return ERROR_SOCKET;
        }
        data->senders[i].args.socket_fd = sock;
    }

    return 0;
}

static int setup_threads(net_data_t* data, size_t num_senders) {
    for (size_t i = 0; i < num_senders; ++i) {
        pthread_t* thread = &(data->senders[i].thread);
        void* args = &(data->senders[i].args);
        int res = pthread_create(thread, NULL, sender_f, args);
        if (res) {
            return ERROR_THREAD;
        }
    }

    return 0;
}


/**
 * @brief Initialize sender threads and their sockets.
 * 
 * @param data Global data structure containing the array of threads and the sockets.
 * @param num_senders Number of threads and sockets (equal).
 * @return int 0 if success, non-0 otherwise.
 */
int init_senders(net_data_t* data, size_t num_senders) {
    /* --- ALLOCATE SENDERS --- */
    // As many sockets as there are threads
	int res = setup_sockets(data, num_senders);
    if (res) {
        return res;
    }

    /* --- START THREADS --- */
    res = setup_threads(data, num_senders);
    if (res) {
        return res;
    }

    return 0;
}


/**
 * @brief Immediately terminate all sender threads.
 * 
 * @param data Global data structure containing the array of threads and the sockets.
 * @param num_senders Number of threads and sockets (equal).
 */
void terminate_senders(net_data_t* data, size_t num_senders) {
    for (size_t i = 0; i < num_senders; ++i) {
        // send signal to all threads
    }
    for (size_t i = 0; i < num_senders; ++i) {

    }
}