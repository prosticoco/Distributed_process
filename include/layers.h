#pragma once

#include "data.h"


#define LINE_MAX_LENGTH 8

/**
 * @brief fair loss send
 * 
 * @param data ptr to all data we need
 * @param dest_pid pid of process we want to send msg
 * @param msg msg we want to send
 * @return int 
 */
int send_fl(net_data_t* data, int socket_fd, unsigned int dest_pid, msg_t msg);

/**
 * @brief fair loss deliver
 * 
 * @param data all data we need
 * @param thread_idx
 * @param msg message we received
 * @return int 
 */
int deliver_fl(net_data_t* data, msg_t msg);

/**
 * @brief perfect link send
 * 
 * @param pid pid of process we send to
 * @param data all data we need
 * @param msg msg to send
 * @return int 
 */
int send_pl(unsigned int pid, int socket_fd, net_data_t* data, msg_t msg);

/**
 * @brief perfect link deliver
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int deliver_pl(net_data_t* data, msg_t msg);

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int send_beb(net_data_t* data, urb_msg_t msg);

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int deliver_beb(net_data_t* data, urb_msg_t msg);

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int send_urb(net_data_t* data, fifo_msg_t msg,lcb_msg_t lcbmsg);

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int deliver_urb(net_data_t* data, urb_msg_t msg);

/**
 * @brief FIFO broadcasts
 * 
 * @param data we need about our proc
 * @param m sequence of messages to broadcast (1...m)
 * @return int 0 if successful, error value else
 */
int send_fifo(net_data_t* data, int m);

/**
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int deliver_fifo(net_data_t* data, fifo_msg_t msg);

/**
 * @brief Send with localized causal broadcast, a given number of messages.
 * 
 * @param data The process local network data.
 * @param m The number of messages to send.
 * @return int 0 on case of success, non-zero otherwise.
 */
int send_lcb(net_data_t* data, int m);

/**
 * @brief Deliver a message with localized causal broadcast.
 * 
 * @param data The process local network data.
 * @param msg The message to deliver.
 * @return int 0 in case of success, non-zero otherwise.
 */
int deliver_lcb(net_data_t* data, lcb_msg_t msg);

// method will write that a message was urb broadcast from this current node
// method is defined in urb.c
int log_urb_broadcast(net_data_t* data,fifo_msg_t msg);


int log_deliver_lcb(net_data_t* data , unsigned int pid, unsigned int seq_num);

/**
 * @brief function to serialize the message structure into a buffer of bytes
 *          to be sent over the network 
 * 
 * @param msg the message to serialize
 * @param buffer the automatically allocated and updated buffer
 * @param num_proc the number of nodes in the network
 * @return int return negative code on error
 */
int serialize(msg_t* msg, char** buffer,size_t num_proc);

/**
 * @brief inverse of the serialization function, however the buffer must already be allocated
 * 
 * @param msg empty message that will be updated, note the vector will be allocated in the function
 * @param buffer the buffer received from the network
 * @param num_proc the number of nodes in the network
 * @return int return negative code on error
 */
int deserialize(msg_t * msg,char* buffer,size_t num_proc);

/**
 * @brief function to help debug, it simply prints all the fields of a given message
 * 
 * @param msg the message to be printed
 * @return int should always return 0, otherwise it will perform a seg fault
 */
int print_message(msg_t msg);

/**
 * @brief Function to send messages over the network, encapsulates the serialization process
 * 
 * @param data the current process data
 * @param socket_fd file descriptor of the sending socket
 * @param dest_pid the node that will receive the sent message
 * @param msg the message to be serialized and sent
 * @return int returns negative code on error
 */
int send_message(net_data_t* data,int socket_fd,unsigned int dest_pid,msg_t msg);

/**
 * @brief function to receive a message over the network
 * 
 * @param data the data of the current process
 * @param buffer the pre-allocated buffer to contain the receiving message
 * @param next_message empty message will be updated after buffer reception
 * @return int returns a negative code on error
 */
int receive_msg(net_data_t* data,char* buffer, msg_t* next_message);

/**
 * @brief returns the bytesize of the message
 * 
 * @param num_proc the number of processes
 * @return size_t 
 */
size_t message_size(size_t num_proc);

size_t num_fields(size_t num_proc);

size_t num_static();

