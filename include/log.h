#pragma once

#include "data.h"

/**
 * @brief initializes a buffer which will be written to the output file
 * whenever it gets full
 * 
 * @param logdata structure for the buffer 
 * @param buf_max_size  max buffer size
 * @param filename filename to write output
 * @return int 0 on success negative code on error
 */
int init_log_data(log_data_t* logdata,size_t buf_max_size,char* filename);

/**
 * @brief function to write a line to the logs
 * 
 * @param logdata the log structure
 * @param line the line to write
 * @param size  the size of the line
 * @return int 0 upon success, negative code upon error
 */
int write_log(log_data_t* logdata, char* line,size_t size);

/**
 * @brief this function is called by write_log whenever the buffer is full
 * it writes the buffer to the output file
 * 
 * @param logdata the log structure
 * @return int 0 upon success, negative code upon error
 */
int write_to_file(log_data_t* logdata);

/**
 * @brief frees the structure, ie buffer used to store the logs
 * 
 * @param logdata the log structure to free
 * @return int should return 0 
 */
int free_log_data(log_data_t* logdata);

/**
 * @brief function called by fifo whenever a message can be delivered
 * this function will call the above functions to write to the log file
 * 
 * @param data pointer to the processes data
 * @param msg the message to deliver
 * @return int 0 on success negative code otherwise
 * 
 */
int log_deliver(net_data_t* data,fifo_msg_t msg);