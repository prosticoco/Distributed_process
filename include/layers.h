#include "data.h"

/**
 * @brief fair loss send
 * 
 * @param data ptr to all data we need
 * @param dest_pid pid of process we want to send msg
 * @param msg msg we want to send
 * @return int 
 */
int send_fl(net_data_t* data, unsigned int dest_pid, msg_t msg);

/**
 * @brief fair loss deliver
 * 
 * @param data all data we need
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
int pl_send(unsigned int pid, net_data_t* data, msg_t msg);

/**
 * @brief perfect link deliver
 * 
 * @param data 
 * @param msg 
 * @return int 
 */
int pl_deliver(net_data_t* data, msg_t msg);