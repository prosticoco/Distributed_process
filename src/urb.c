
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "data.h"
#include "urb.h"

#define MAX_SIZE_URB 1024*8


init_urb_table(urb_table_t* table, unsigned int no_msgs, unsigned int no_process){
    table->total_entries = no_msgs*no_process;
    table->values = calloc(table->total_entries,sizeof(urb_entry_t));
    if(table->values == NULL){
        return ERROR_MEMORY;
    }
    table->no_msgs = no_msgs;
    table->no_process = no_process;
    return 0;
}


is_delivered_urb(urb_table_t* table, unsigned int seen_id){
    if(seen_id >= table->total_entries){
        return 0;
    }
    return table->values[seen_id].delivered;
}

add_delivered_urb(urb_table_t* table,unsigned int seen_id){
    if(seen_id > MAX_SIZE_URB){
        return ERROR_TABLE;
    }
    while(seen_id >= table->total_entries){
        unsigned int old_size = table->total_entries;
        unsigned int new_size = table->total_entries*2;

    }
}

is_seen_urb(urb_table_t* table,unsigned int seen_id);

add_seen_urb(urb_table_t* table, unsigned int seen_id);

free_urb_table(urb_table_t* table);