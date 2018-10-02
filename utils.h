#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "da_proc.h"

int parse_membership_args(int argc, char** argv, int* total_process_number, da_process_t* this_process);

#endif