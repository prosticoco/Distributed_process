#include <stdio.h>

#include "data.h"
#include "mqueue.h"
#include "layers.h"
#include "urb.h"

//BEB broadcast
/* for every message we want to broadcast
*  we add a task in the task queue where it will be
*  processed by a sender thread as a pl_send
*/
int send_beb(net_data_t* data, urb_msg_t msg) {
    unsigned int num_proc = data->num_proc;
    unsigned int self = data->self_pid;

    // Send message to all processes
    for (size_t i = 1; i <= num_proc; ++i){
        int res;
        //avoid sending message to self
        if(i != self){
            //wrap urb message in msg
            msg_t message;
            message.mid = (msg.seen_id)*num_proc + self -1;
            message.ackid = (msg.seen_id)*num_proc + i -1;
            message.mtype = 0;
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

//Beb delivery just calls urb delivery (keeping it layered)
int deliver_beb(net_data_t* data, urb_msg_t msg) {
    return deliver_urb(data, msg);
}