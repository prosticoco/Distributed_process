#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include "structure.h"
#include "error.h"
#include "utils.h"


static int wait_for_start = 1;
static int total_process_number = 0;
static int total_msg_number = 0;
static da_process_t this_process;
// Matrix of acks from other processes
static bool** ack_matrix = NULL;
// Tells us if each sender thread has to send an ACK
static bool* acks_to_send = NULL;


static void free_resources(void) {
	if (ack_matrix != NULL) {
		free_ack_matrix(ack_matrix, total_process_number);
	}
	if (acks_to_send != NULL) {
		free_acks_to_send(acks_to_send);
	}
}

// function called when process recieves start signal
// process will broadcast messages to other processes
static void start(int signum) {
	wait_for_start = 0;


}

static void stop(int signum) {
	// Reset signal handlers to default
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);


	// Immediately stop network packet processing
	printf("Immediately stopping network packet processing.\n");

	// Write/flush output file if necessary
	printf("Writing output.\n");

	// Free resources
	free_resources();

	// Exit directly from signal handler
	exit(0);
}

int main(int argc, char** argv) {

	// Set signal handlers
	signal(SIGUSR1, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);
	// set signal masks for threads not responsible of handling signals
	// Parse arguments, including membership
	int res = parse_membership_args(argc, argv, &total_process_number, &total_msg_number, &this_process);
	// Initialize ack matrix (N - 1) x M
	res += initialize_ack_matrix(&ack_matrix, total_process_number, total_msg_number);
	// Initialize acks-to-send array
	res += initialize_acks_to_send(&acks_to_send, total_process_number);
	if (res != 0) {
		free_resources();
		return res;
	}

	// Initialize application
	// Start listening for incoming UDP packets
	printf("Initializing.\n");


	// Wait until start signal
	while(wait_for_start) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 0;
		sleep_time.tv_nsec = 1000;
		nanosleep(&sleep_time, NULL);
	}


	// Broadcast messages
	printf("Broadcasting messages.\n");


	// Wait until stopped
	while(1) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 1;
		sleep_time.tv_nsec = 0;
		nanosleep(&sleep_time, NULL);
	}
}
