#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "parser.h"
#include "error.h"

static int cleanup(FILE* file, int result) {
    if (file) {
        fclose(file);
    }
    return result;
}

/**
 * @brief Fill global data structure using arguments and membership file.
 * 
 * @param argc argc from main.
 * @param argv argv from main.
 * @param data The data structure to fill.
 * @return int 0 in case of success, non-0 otherwise.
 */
int parse_membership_args(int argc, char** argv, net_data_t* data) {
    // Check arguments
    if (argc < 4) {
        fprintf(stderr, "Error: parsing: not enough arguments\n");
        return cleanup(NULL, ERROR_ARGS);
    }

    // ----- GET CURRENT PROCESS ID -----
    size_t self_pid = (size_t) atoi(argv[1]);
    if (self_pid == 0) {
        fprintf(stderr, "Error: parsing: invalid process number\n");
        return cleanup(NULL, ERROR_ARGS);
    }
    data->self_pid = self_pid;

    // ----- GET NUMBER OF MESSAGES TO SEND -----
    int num_msg = atoi(argv[3]);
    if (num_msg == 0) {
        fprintf(stderr, "Error: parsing: invalid number of messages to send\n");
        return cleanup(NULL, ERROR_ARGS);
    }
    data->num_msg = num_msg;

    // Open file
    FILE* membership_file = fopen(argv[2], "r");
    if (!membership_file) {
        fprintf(stderr, "Error: parsing: could not open membership file\n");
        return cleanup(membership_file, ERROR_FILE);
    }

    // ----- GET TOTAL NUMBER OF PROCESSES -----
    char line[32];
    if (!fgets(line, sizeof(line), membership_file)) {
        fprintf(stderr, "Error: parsing: could not read membership file\n");
        return cleanup(membership_file, ERROR_FILE);
    }
    size_t num_proc = (size_t) atoi(line);
    if (num_proc == 0) {
        fprintf(stderr, "Error: parsing: invalid total process number\n");
        return cleanup(membership_file, ERROR_FILE);
    }
    data->num_proc = num_proc;

    // ----- FILL ADDRESS BOOK -----
    data->address_book = alloc_addr_book(num_proc);
    if (!data->address_book) {
        fprintf(stderr, "Error: parsing: could not allocate address book\n");
        return cleanup(membership_file, ERROR_MEMORY);
    }
    while (fgets(line, sizeof(line), membership_file)) {
        // Get line process uid, address and port
        const char* pid_str = strtok(line, " ");
        const char* ipv4_str = strtok(NULL, " ");
        const char* port_str = strtok(NULL, " ");
        if (!pid_str || !ipv4_str || !port_str) {
            fprintf(stderr, "Error: parsing: invalid line format\n");
            return cleanup(membership_file, ERROR_FILE);
        }

        size_t pid = (size_t) atoi(pid_str);
        if (pid == 0) {
            fprintf(stderr, "Error: parsing: invalid line format\n");
            return cleanup(membership_file, ERROR_FILE);
        }

        // Help from: https://www.gnu.org/software/libc/manual/html_node/Inet-Example.html
        struct in_addr ipv4;
        if (!inet_aton(ipv4_str, &ipv4)) {
            fprintf(stderr, "Error: parsing: invalid line format\n");
            return cleanup(membership_file, ERROR_FILE);
        }

        uint16_t port = htons((uint16_t) atoi(port_str));

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr = ipv4;

        // Add entry into address book
        if (add_entry(data->address_book, pid, addr)) {
            fprintf(stderr, "Error: parsing: could not add entry to address book\n");
            return cleanup(membership_file, ERROR_FILE);
        }
    }

    return cleanup(membership_file, 0);
}