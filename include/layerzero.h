
#include "data.h"
#include "error.h"
#include "pending.h"

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