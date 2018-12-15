
#include "lcb.h"
#include "layers.h"
#include "pending.h"


int send_lcb(net_data_t* data, int m){
    int error;
    fifo_msg_t fifo_msg;
    lcb_msg_t lcb_msg;
    unsigned int seq;
    for(int i = 1 ; i < m + 1; i++){
        error = get_vec_clock_copy(data,&(lcb_msg.vec_clock));
        if(error){
            printf("Error getting copy of vec_clock in send_lcb \n");
            return error;
        }
        error = update_vec_clock(data,data->self_pid,&seq);
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
    return 0;
}


int deliver_lcb(net_data_t* data, lcb_msg_t msg){
    int error;
    unsigned int seq;
    if(test_vec_clock(data,&(msg.vec_clock),msg.original_sender)){
        error = update_vec_clock(data,msg.original_sender,&seq);
        if(error){
            printf("Error updating vector clock in deliver lcb \n");
            return error;
        }
        error = log_deliver_lcb(data,msg.original_sender,seq);
        if(error){
            printf("Error while log delivering in deliver lcb \n");
            return error;
        }
        error = find_pending(data);
        while(error > 0){
            error = find_pending(data);
        }
        if(error < 0){
            printf("Error in find pending from deliver lcb \n");
            return error;
        }
    }else{
        error = add_pending_lcb(data->lcb_pending,msg.original_sender,msg.vec_clock);
        if(error){
            printf("Error while adding vector to pending set \n");
            return error;
        }
    }
    error = destroy_vector(&(msg.vec_clock));
    if(error){
        printf("error while destroying vector clock in deliver lcb \n");
        return error;
    }
    return 0;
}


int log_deliver_lcb(net_data_t* data,unsigned int pid, unsigned int seq){
    fifo_msg_t fifo;
    fifo.original_sender = pid;
    fifo.sequence_num = seq;
    int error = log_deliver(data,fifo);
    if(error){
        printf("Error delivering lcb \n");
        return error;
    }
    return 0;
}
