#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "da_proc.h"

static int wait_for_start = 1;

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

	// Exit directly from signal handler
	exit(0);
}

int main(int argc, char** argv) {

	// Set signal handlers
	signal(SIGUSR1, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);


	// Parse arguments, including membership
	if (argc < 4) {
		fprintf(stderr, "ERROR: not enough arguments.\n");
		return 1;
	}

	int uid = atoi(argv[1]);
	printf("Process uid = %i\n", uid);

	char * membership_filename = argv[2];
	printf("Membership filename = \"%s\"\n", membership_filename);

	int messages_to_broadcast = atoi(argv[3]);
	printf("Number of messages to broadcast = %i\n", messages_to_broadcast);
	if (messages_to_broadcast < 0) {
		fprintf(stderr, "ERROR: #messages to broadcast cannot be negative.\n")
	}

	// Initialize application
	// Start listening for incoming UDP packets
	printf("Initializing.\n");


	//wait until start signal
	while(wait_for_start) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 0;
		sleep_time.tv_nsec = 1000;
		nanosleep(&sleep_time, NULL);
	}


	//broadcast messages
	printf("Broadcasting messages.\n");


	//wait until stopped
	while(1) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 1;
		sleep_time.tv_nsec = 0;
		nanosleep(&sleep_time, NULL);
	}
}
