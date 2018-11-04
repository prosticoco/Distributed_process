#pragma once

/**
 * @brief Fill global data structure and allocate underlying address book
 *        using arguments and membership file.
 * 
 * @param argc argc from main.
 * @param argv argv from main.
 * @param data The data structure to fill.
 * @return int 0 in case of success, non-0 otherwise.
 */
int parse_membership_args(int argc, char** argv, net_data_t* data);