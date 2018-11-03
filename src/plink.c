#include <stdlib.h>
#include "error.h"
#include "plink.h"

#define MAX_SIZE 1024

int init_table_uid(uid_table_t* table,unsigned int no_entries, unsigned int no_process){
}

int table_read_entry(uid_table_t* table,mid_t mid){
    if(mid.sn > table->no_entries){
        return 0;
    }
    return table->entries[mid.sn][mid.original_sender][mid.sender];
}

int table_write_entry(uid_table_t* table,mid_t mid,unsigned int value){
    while(mid.sn > table->no_entries){

    }
}

int free_table_uid(uid_table_t* table);