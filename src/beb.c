#include <stdio.h>

#include "data.h"
#include "mqueue.h"
#include "layers.h"
#include "urb.h"

int send_beb(net_data_t* data, urb_msg_t msg) {
    size_t num_proc = data->address_book->num_proc;
    unsigned int self = data->self_pid;
    // Send message to all processes
    for (size_t i = 0; i < num_proc; ++i) {
        // Get IP address for process i
        // TODO: maybe avoid broadcasting to ourselves ?
        // This would require knowing our own pid.
        int res;
        if(i != self){
            msg_t message;
            message.mid = (num_proc*num_proc*(msg.seen_id)+ num_proc*(msg.no_seen) + self);
            message.mtype = 1;
            message.sender = self;
            message.urb_msg = msg;
            queue_task_t task;
            task.pid_dest = i;
            task.msg = message;
            res = enqueue(data->task_q, &task);
            if(res){
                return res;
            }
        }
        
    }
    return 0;
}


int deliver_beb(net_data_t* data, urb_msg_t msg) {
    return deliver_urb(data, msg);
}