#include <stdlib.h>
#include <string.h>

#include "urb.h"
#include "data.h"
#include "error.h"
#include "layers.h"
#include "log.h"
#include "plink.h"

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
    unsigned int seen_id = (data->num_proc)*(msg.sequence_num -1) + msg.original_sender -1;
    // add the message to the seen messages
    int error = add_seen_urb(data->urb_table,seen_id);
    if(error){
        printf("Error in add:seen URB SEND\n");
        return error;
    }
    error = log_urb_broadcast(data,msg);
    if(error){
        return error;
    }
    urb_msg_t new_msg;
    new_msg.fifo_msg = msg;
    new_msg.no_seen = 1;
    new_msg.seen_id = seen_id;
    error = send_beb(data,new_msg);
    if(error){
        printf("URB SEND error in send babe\n");
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
    
    int error;
    if(!is_seen_urb(data->urb_table,msg.seen_id)){
        
        error = add_seen_urb(data->urb_table,msg.seen_id);
        if(error){
            printf("URB : Error in add_seen_urb\n");
            return error;
        }
        urb_msg_t new_msg = msg;
        new_msg.no_seen = msg.no_seen + 1;
        error = send_beb(data,new_msg);
        if(error){
            printf("URB : Error in send babe\n");
            return error;
        }
    }
    if((msg.no_seen > ((data->num_proc) / 2)) &&
     !is_delivered_urb(data->urb_table,msg.seen_id)){
         error = add_delivered_urb(data->urb_table,msg.seen_id);
         if(error){
             printf("URB : Error in add_delivered\n");
             return error;
         }
         deliver_fifo(data,msg.fifo_msg);
    }
    return 0;
}


int log_urb_broadcast(net_data_t* data,fifo_msg_t msg){
    int error;
    unsigned int snr = msg.sequence_num;
    char line[LINE_MAX_LENGTH];
    error = sprintf(line,"b %u\n",snr);
    if(error < 0){
        return ERROR_IO;
    }
    size_t size = strlen(line);
    error = write_log(data->logdata,line,size);
    if(error){
        return error;
    }
    return 0;
}


// interface for ack[m] table 

int init_ack_urb(urb_ack_table_t* delivered,unsigned int no_entries, unsigned int no_process){
    int error = init_table_uid(&(delivered->table),no_entries, no_process);
    if(error){
        printf("Error initializing delivered table\n");
        return error;
    }
    return 0;
}

int set_ack_urb(urb_ack_table_t* delivered,mid_t mid){
    int error = table_write_entry(&(delivered->table),mid,1);
    if(error){
        printf("Error writing in delivered table\n");
        return error;
    }
    return 0;
}

int free_ack_urb(urb_ack_table_t* delivered){
    free(delivered->table.entries);
    return 0;
}

int can_deliver(urb_ack_table_t* delivered,unsigned int seen_id){
    unsigned int no_proc = delivered->table.no_process;
    if(no_proc*seen_id + no_proc >= delivered->table.total_entries){
        return 0;
    }
    size_t counter = 0;
    for(int i = seen_id; i < (seen_id + no_proc); i++){
        if(delivered->table.entries[i] == 1){
            counter += 1;
        }
    }
    if(counter > (no_proc / 2)){
        return 1;
    }
    return 0;
}