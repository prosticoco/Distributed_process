#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

/**
 * @brief Parses command line arguments and updates the given variables.
 * 
 * @param argc Number of command line arguments.
 * @param argv Command line arguments array.
 * @param total_process_number Pointer to the total number of processes.
 * @param total_msg_number Pointer to the total number of messages to send.
 * @param this_process Pointer to the structure of the calling process.
 * @return int Success: 0, Failure: not 0
 */
int parse_membership_args(int argc, char** argv, int* total_process_number, int* total_msg_number, da_process_t* this_process) {
    if (argc < 4) {
		fprintf(stderr, "ERROR: not enough arguments.\n");
		return 1;
	}

	int uid = atoi(argv[1]);
	printf("Process uid = %i\n", uid);
	this_process->uid = uid;

	const char* membership_filename = argv[2];
	printf("Membership filename = \"%s\"\n", membership_filename);

	*total_msg_number = atoi(argv[3]);
	printf("Number of messages to broadcast = %i\n", *total_msg_number);

	FILE* membership_file = fopen(membership_filename, "r");
	if (membership_file == NULL) {
		fprintf(stderr, "ERROR: Could not open membership file.\n");
		return 1;
	}

	// 12 + 3 + 4 + 3 + 5 = 27 -> 32 is enough for one line here.
	char line[32];
	// Get total number of processes from the 1st line
	if (fgets(line, sizeof(line), membership_file) == NULL) {
		fprintf(stderr, "ERROR: Could not read from membership file.\n");
		fclose(membership_file);
		return 1;
	}
	*total_process_number = atoi(line);
	printf("Total number of processes = %i\n", *total_process_number);

	bool found = false;

	// Iterate over lines until we find the right line
	while (fgets(line, sizeof(line), membership_file) != NULL) {
		// Get line process uid
		char* puid_str = strtok(line, " ");
		// Check that this is the line for the process
		if (puid_str != NULL && atoi(puid_str) == uid) {
			// Parse IPv4 address and port number
			const char* ipv4_addr_str = strtok(NULL, " ");
			const char* port_num_str = strtok(NULL, " ");
			if (ipv4_addr_str == NULL || port_num_str == NULL ||
				!inet_aton(ipv4_addr_str, &(this_process->ipv4_addr))) {
				fprintf(stderr, "ERROR: line for process has invalid format.\n");
				fclose(membership_file);
				return 1;
			}
			this_process->port_num = (unsigned short) atoi(port_num_str);

			printf("Process IPv4 = %s\n", ipv4_addr_str);
			printf("Process port number = %hu\n", this_process->port_num);

			found = true;
			break;
		}
	}

	if (!found) {
		fprintf(stderr, "ERROR: no line matching the provided pid was found.\n");
		fclose(membership_file);
		return 1;
	}

	fclose(membership_file);
    return 0;
}

/**
 * @brief Allocate memory for the ack matrix and set all flags to false.
 * 
 * @param acks Pointer to the ack matrix.
 * @param total_process_number Total number of processes.
 * @param total_msg_number Total number of messages to send.
 * @return int Success: 0, Failure: not 0
 */
int initialize_ack_matrix(bool*** acks, int total_process_number, int total_msg_number) {
	// Initialize 2-D acks array (N x M), where M = #processes - 1, M = #msg to send for this thread
	bool** acks_tmp = calloc(total_process_number - 1, sizeof(bool*));
	if (acks_tmp == NULL) {
		fprintf(stderr, "ERROR: Could not initialize ack array.\n");
		return 1;
	}
	for (size_t i = 0; i < total_process_number - 1; ++i) {
		// Calloc should initialize everything to false.
		acks_tmp[i] = calloc(total_msg_number, sizeof(bool));
		if (acks_tmp[i] == NULL) {
			fprintf(stderr, "ERROR: Could not initialize ack array.\n");
			return 1;
		}
	}

	*acks = acks_tmp;
	return 0;
}

/**
 * @brief Free ack matrix resources.
 * 
 * @param acks The ack matrix.
 * @param total_process_number The total number of processes.
 */
void free_ack_matrix(bool** acks, int total_process_number) {
	for (size_t i = 0; i < total_process_number - 1; ++i) {
		free(acks[i]);
	}
	free(acks);
}

/**
 * @brief Allocate memory for the acks-to-send array and set all flags to false.
 * 
 * @param acks_to_send Pointer to the acks-to-send array.
 * @param total_process_number Total number of processes.
 * @return int Success: 0, Failure: not 0
 */
int initialize_acks_to_send(bool** acks_to_send, int total_process_number) {
	bool* acks_to_send_tmp = calloc(total_process_number - 1, sizeof(bool));
	if (acks_to_send_tmp == NULL) {
		return 1;
	}

	*acks_to_send = acks_to_send_tmp;
	return 0;
}

/**
 * @brief Free acks-to-send array resources.
 * 
 * @param acks_to_send The acks-to-send array.
 */
void free_acks_to_send(bool* acks_to_send) {
	free(acks_to_send);
}
