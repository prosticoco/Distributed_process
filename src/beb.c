#include <stdio.h>

#include "beb.h"
#include "urb.h"

int beb_send(beb_data_t* data, beb_msg_t* msg) {
    size_t num_proc = data->address_book->num_proc;
    // Send message to all processes
    for (size_t i = 0; i < num_proc; ++i) {
        // Get IP address for process i
        // TODO: maybe avoid broadcasting to ourselves ?
        // This would require knowing our own pid.
        struct sockaddr_in destination;
        int res = get_addr(data->address_book, i, &destination);
        if (res) {
            fprintf(stderr, "Error: could not retrieve address for pid %zu.", i);
            return res;
        }
        res = pl_send(data, msg);
        // TODO: handle when sending the message failed (but not blocked)
    }
    return 0;
}

/*
int beb_deliver() {
    return urb_deliver();
}*/