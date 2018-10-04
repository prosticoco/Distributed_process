#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "structure.h"

int parse_membership_args(int argc, char** argv, int* total_process_number, int* total_msg_number, da_process_t* this_process);

int initialize_ack_matrix(ack_matrix_t* ack_matrix, int total_process_number, int total_msg_number);

void free_ack_matrix(ack_matrix_t ack_matrix, int total_process_number);

int initialize_acks_to_send(ack_list_t* acks_to_send, int total_process_number);

void free_acks_to_send(ack_list_t acks_to_send);

#endif