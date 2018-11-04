
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "data.h"
#include "urb.h"


int init_table_uid(uid_table_t* table ,unsigned int no_msgs, unsigned int no_process){
    table->entries = calloc(no_msgs*no_process*no_process,sizeof(unsigned int));
    if(table->entries == NULL){
        return ERROR_MEMORY;
    }
    table->no_msgs = no_msgs;
    table->no_process = no_process;
    table->total_entries = no_msgs*no_process*no_process;
    return 0;
}

int table_read_entry(uid_table_t* table,mid_t mid){
    if(mid >= table->total_entries){
        return 0;
    }
    return table->entries[mid];
}

int table_write_entry(uid_table_t* table,mid_t mid,unsigned int value){
    unsigned int new_size;
    unsigned int old_size = table->total_entries;
    while(mid >= table->total_entries){
        new_size = table->total_entries * 2;
        table->entries = realloc(table->entries,sizeof(unsigned int)*new_size);
        if(table->entries == NULL){
            return ERROR_MEMORY;
        }     
        table->total_entries = new_size;
        memset(&(table->entries[old_size]),0,table->total_entries - old_size);
        table->no_msgs = table->no_msgs*2;
        return 0;
    }
    table->entries[mid] = value;
    return 0;
}

int free_table_uid(uid_table_t* table){
    free(table->entries);
    table->entries = NULL;
    return 0;
}

int urb_send(urb_data_t* data, urb_msg_t* msg){
    int error;
    //add self to pending
    if(error=table_write_entry(data->pending, msg->beb_msg->mid, SELF) < 0){
        return error;
    }
    error = beb_send(msg->beb_msg);
    return error;

}


int urb_deliver(urb_data_t* data, urb_msg_t msg){
    int error;
    if(error = write_ack_table(msg->beb_msg->mid, msg->sender_pid) < 0){
        return error;
    }//tout ce que je fais cest du caca
    /** urb deliver
     * add self to ack entry
     * if not in pending
     * add to pending
     * send beb
     * 
     * if pending and candeliver but message not in delivered
     * add message to delivered
     * call fifo deliver/log deliver
     */

}

int candeliver(urb_data_t data, urb_msg_t* msg){
    int error;
    if(error = read_ack_table(msg->beb_msg->mid) < 0){
        return error;
    }
    N = data->address_book->size;
    if(error > N/2){
        return 1;
    }
    else{
        return 0;
    }
    return error;
}
