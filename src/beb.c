#include <stdio.h>

#include "data.h"
#include "mqueue.h"
#include "layers.h"
#include "urb.h"

int send_beb(net_data_t* data, urb_msg_t msg) {
    unsigned int num_proc = data->num_proc;
    unsigned int self = data->self_pid;
    // Send message to all processes
    for (size_t i = 1; i <= num_proc; ++i){
        // Get IP address for process i
        // TODO: maybe avoid broadcasting to ourselves ?
        // This would require knowing our own pid.
        int res;
        
        if(i != self){
            printf("Bebby broadcasting to process no %zu \n",i);
            msg_t message;
            message.mid = (num_proc * (msg.seen_id)  + self-1);
            message.mtype = 0;
            message.sender = self;
            message.urb_msg = msg;
            queue_task_t task;
            task.pid_dest = i;
            task.msg = message;
            res = enqueue(data->task_q, &task);
            if(res){
                printf("COULD NOT PUT SHIT ON MY DICK\n");
                return res;
            }
        }
        
    }
    return 0;
}


int deliver_beb(net_data_t* data, urb_msg_t msg) {
    return deliver_urb(data, msg);
}