#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "structure.h"

int parse_membership_args(int argc, char** argv, receiver_info_t* data);

int initialize_acks_to_send(ack_list_t* acks_to_send, int total_process_number);

void free_acks_to_send(ack_list_t acks_to_send);

bool get_msg_ack(size_t process_index, size_t msg_index);

#endif
