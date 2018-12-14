
#include "lcb.h"
#include "layers.h"
#include "pending.h"


int send_lcb(net_data_t* data, int m){
    int error;
    fifo_msg_t fifo_msg;
    lcb_msg_t lcb_msg;
    for(int i = 1 ; i <m + 1; i++){
        error = get_vec_clock_copy(data,&(lcb_msg.vec_clock));
        if(error){
            printf("Error getting copy of vec_clock in send_lcb \n");
            return error;
        }
        error = update_vec_clock(data,data->self_pid);
        if(error){
            printf("Error updating self vec clock in send_lcb\n");
            return error;
        }
        fifo_msg.original_sender = data->self_pid;
        fifo_msg.sequence_num = i;
        lcb_msg.original_sender = data->self_pid;
        error = send_urb(data,fifo_msg,lcb_msg);
        if(error){
            printf("Error sending via urb : in send lcb \n");
            return error;
        }
    }
}


int deliver_lcb(net_data_t* data, lcb_msg_t msg){
    int error;
    if(test_vec_clock(data,&(msg.vec_clock),msg.original_sender)){

    }else{
        error = add_pending_lcb(data->lcb_pending,msg.original_sender,msg.vec_clock);
    }

}

