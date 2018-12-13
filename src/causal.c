#include "causal.h"
#include "data.h"
#include "error.h"

/**
 * @brief Send with localized causal broadcast, a given number of messages.
 * 
 * @param data The process local network data.
 * @param m The number of messages to send.
 * @return int 0 on case of success, non-zero otherwise.
 */
int send_LCB(net_data_t* data, int m) {
    // TODO
    return 0;
}

/**
 * @brief Deliver a message with localized causal broadcast.
 * 
 * @param data The process local network data.
 * @param msg The message to deliver.
 * @return int 0 in case of success, non-zero otherwise.
 */
int deliver_LCB(net_data_t* data, lcb_msg_t msg) {
    // TODO
    return 0;
}