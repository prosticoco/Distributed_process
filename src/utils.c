#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "utils.h"
#include "error.h"

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
int parse_membership_args(int argc, char** argv, receiver_info_t* data) {
    if (argc < 4) {
        fprintf(stderr, "ERROR: not enough arguments.\n");
        return ERROR_ARGS;
    }

    // ***** this process uid *****
    int uid = atoi(argv[1]);
    printf("Process uid = %i\n", uid);
    data->nodeid = uid;

    // ***** open membership file *****
    const char* membership_filename = argv[2];
    printf("Membership filename = \"%s\"\n", membership_filename);
    // TODO: what do I do with the number of messages to send ? (argv[3])
    FILE* membership_file = fopen(membership_filename, "r");
    if (membership_file == NULL) {
        fprintf(stderr, "ERROR: Could not open membership file.\n");
        return ERROR_FILE;
    }

    // ***** total number of processes *****
    // 12 + 3 + 4 + 3 + 5 = 27 -> 32 is enough for one line here.
    char line[32];
    // Get total number of processes from the 1st line
    if (fgets(line, sizeof(line), membership_file) == NULL) {
        fprintf(stderr, "ERROR: Could not read from membership file.\n");
        fclose(membership_file);
        return ERROR_FILE;
    }
    // TODO: -1 ?
    data->no_nodes = atoi(line) - 1;
    printf("Total number of processes = %i\n", data->no_nodes);

    // ***** get IPv4 address for current process *****
    bool found = false;
    // Iterate over lines until we find the right line

    // ***** other processes infos *****
    /*
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
    */

    fclose(membership_file);
    return 0;
}


/**
 * @brief Allocate memory for the acks-to-send array and set all flags to false.
 * 
 * @param acks_to_send Pointer to the acks-to-send array.
 * @param total_process_number Total number of processes.
 * @return int Success: 0, Failure: not 0
 */
int initialize_acks_to_send(ack_list_t* acks_to_send, int total_process_number) {
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
void free_acks_to_send(ack_list_t acks_to_send) {
    free(acks_to_send);
}
