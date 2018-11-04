#include <stdlib.h>
#include <string.h>

#include "urb.h"
#include "data.h"
#include "error.h"
#include "layers.h"

#define MAX_SIZE_URB 1024*8


int init_urb_table(urb_table_t* table, unsigned int no_msgs, unsigned int no_process){
    table->total_entries = no_msgs*no_process;
    table->values = calloc(table->total_entries,sizeof(urb_entry_t));
    if(table->values == NULL){
        return ERROR_MEMORY;
    }
    table->no_msgs = no_msgs;
    table->no_process = no_process;
    return 0;
}

int realloc_table(urb_table_t* table){
    unsigned int old_size = table->total_entries;
    unsigned int new_size = table->total_entries*2;
    table->values = realloc(table->values,new_size);
    if(table->values == NULL){
        return ERROR_MEMORY;
    }
    memset(&(table->values[old_size]),0,new_size - old_size);
    table->total_entries *= 2;
    table->no_msgs *= 2;
    return 0;
}


int is_delivered_urb(urb_table_t* table, unsigned int seen_id){
    if(seen_id >= table->total_entries){
        return 0;
    }
    return table->values[seen_id].delivered;
}

int add_delivered_urb(urb_table_t* table,unsigned int seen_id){
    int error;
    if(seen_id > MAX_SIZE_URB){
        return ERROR_TABLE;
    }
    while(seen_id >= table->total_entries){
        error = realloc_table(table);
        if(error){
            return error;
        }
    }
    table->values[seen_id].delivered = 1;
    return 0;
}

int is_seen_urb(urb_table_t* table,unsigned int seen_id){
    if(seen_id >= table->total_entries){
        return 0;
    }
    return table->values[seen_id].seen;
}

int add_seen_urb(urb_table_t* table, unsigned int seen_id){
    int error;
    if(seen_id > MAX_SIZE_URB){
        return ERROR_TABLE;
    }
    while(seen_id >= table->total_entries){
        error = realloc_table(table);
        if(error){
            return error;
        }    
    }
    table->values[seen_id].seen = 1;
    return 0;
}



int free_urb_table(urb_table_t* table){
    free(table->values);
    table->values = NULL;
    return 0;
}


/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int send_urb(net_data_t* data, fifo_msg_t msg){
    // compute the URB's unique id
    unsigned int seen_id = (data->total_no_process)*(msg.sequence_num -1) + msg.original_sender -1;
    // add the message to the seen messages
    int error = add_seen_urb(data->urb_table,seen_id);
    if(error){
        return error;
    }
    urb_msg_t new_msg;
    new_msg.fifo_msg = msg;
    new_msg.no_seen = 1;
    new_msg.seen_id = seen_id;
    error = send_beb(data,new_msg);
    if(error){
        return error;
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int deliver_urb(net_data_t* data, urb_msg_t msg){
    urb_msg_t new_msg;
    int error;
    if(!is_seen_urb(data->urb_table,msg.seen_id)){
        error = add_seen_urb(data->urb_table,msg.seen_id);
        if(error){
            return error;
        }
        msg.no_seen += 1;
        error = send_beb(data,msg);
        if(error){
            return error;
        }
    }
    if((msg.no_seen > (data->total_no_process) / 2) &&
     !is_delivered_urb(data->urb_table,msg.seen_id)){
         error = add_delivered_urb(data->urb_table,msg.seen_id);
         if(error){
             return error;
         }
         deliver_fifo(data,msg.fifo_msg);
    }

}
