#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#include "log.h"
#include "error.h"



// Inits the logging data, opens a file to write in output, filename should be null-terminated
int init_log_data(log_data_t* logdata,size_t buf_max_size,char* filename){
    logdata->buffer = calloc(buf_max_size,sizeof(char));
    if(logdata->buffer == NULL){
        return ERROR_MEMORY;
    }
    logdata->file = fopen(filename,"a+b");
    if(logdata->file == NULL){
        return ERROR_IO;
    }
    // set initial attributes
    logdata->buf_current_size = 0;
    logdata->buf_max_size = buf_max_size;
    return 0;
}

// writes to the buffer, if the buffer is full then copies the buffer to the file
// the line should end with '\n' 
int write_log(log_data_t* logdata, char* line,size_t size){
    pthread_mutex_lock(&(logdata->loglok));
    int error;
    if(size + logdata->buf_current_size > logdata->buf_max_size){
        error = write_to_file(logdata);
        if(error){
            return error;
        }
    }
    char * ptr = memcpy(&(logdata->buffer[logdata->buf_current_size]),line,size);
    if(ptr == NULL){
        return ERROR_MEMORY;
    }
    logdata->buf_current_size += size;
    pthread_mutex_unlock(&(logdata->loglok));
    return 0;
}

// copies buffer to output file
int write_to_file(log_data_t* logdata){
    if(!fwrite(logdata->buffer,sizeof(char),logdata->buf_current_size,logdata->file)){
        return ERROR_IO;
    }
    memset(logdata->buffer,0,logdata->buf_max_size);
    logdata->buf_current_size = 0;
    return 0;
}

int free_log_data(log_data_t* logdata){
    free(logdata->buffer);
    fclose(logdata->file);
    return 0;
}