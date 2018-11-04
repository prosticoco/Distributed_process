#include "data.h"

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
int deliver_fl(net_data_t* data, size_t thread_idx, msg_t msg);

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
int deliver_pl(net_data_t* data, size_t thread_idx, msg_t msg);

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
 * @brief 
 * 
 * @param data 
 * @param msg 
 * @return int 
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