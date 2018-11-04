
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "data.h"
#include "urb.h"


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
