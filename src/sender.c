#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "data.h"
#include "error.h"
#include "sender.h"

/**
 * @brief Callback function for sender threads.
 * 
 * @param params Function parameters.
 */
void sender_f(void* params) {

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
        void (*callback)(void *) = sender_f;
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