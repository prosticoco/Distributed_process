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
int send_urb(net_data_t* data, fifo_msg_t msg);

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
int send_LCB(net_data_t* data, int m);

/**
 * @brief Deliver a message with localized causal broadcast.
 * 
 * @param data The process local network data.
 * @param msg The message to deliver.
 * @return int 0 in case of success, non-zero otherwise.
 */
int deliver_LCB(net_data_t* data, lcb_msg_t msg);

// method will write that a message was delivered to the log-buffer
// method is defined in fifo.c
int log_deliver(net_data_t* data,fifo_msg_t msg);

// method will write that a message was urb broadcast from this current node
// method is defined in urb.c
int log_urb_broadcast(net_data_t* data,fifo_msg_t msg);

