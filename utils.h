#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "structure.h"

int parse_membership_args(int argc, char** argv, receiver_info_t* data);

int initialize_ack_matrix(ack_matrix_t* ack_matrix, int total_process_number, int total_msg_number);

void free_ack_matrix(ack_matrix_t ack_matrix, int total_process_number);

int initialize_acks_to_send(ack_list_t* acks_to_send, int total_process_number);

void free_acks_to_send(ack_list_t acks_to_send);

bool get_msg_ack(size_t process_index, size_t msg_index);

// write 1 to the corresponding element in the ack matrix according to pid and msgid
int write_ack_matrix(ack_matrix_t* ack_matrix, unsigned int pid,unsigned int msgid);

// reads the corresponding value in the ack matrix
// if the ack is present returns 1 else returns 0
int read_ack_matrix(ack_matrix_t* ack_matrix, unsigned int pid, unsigned int msgid);

#endif
