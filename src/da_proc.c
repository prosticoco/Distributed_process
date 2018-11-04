#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "addrbook.h"
#include "data.h"
#include "parser.h"


/* ----- GLOBAL DATA ----- */
net_data_t net_data = {
	.address_book = NULL,
	.self_pid = 0,
	.pl_acks = NULL,
	.task_q = NULL
};

static int wait_for_start = 1;


static int init_data(int argc, char** argv) {
	return parse_membership_args(argc, argv, &net_data);
}

static void free_data(void) {
	free_addr_book(net_data.address_book);
}

static void start(int signum) {
	wait_for_start = 0;
}

static void stop(int signum) {
	//reset signal handlers to default
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	//immediately stop network packet processing
	printf("Immediately stopping network packet processing.\n");

	//write/flush output file if necessary
	printf("Writing output.\n");

	//exit directly from signal handler
	exit(0);
}

int main(int argc, char** argv) {

	//set signal handlers
	signal(SIGUSR2, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);


	// Parse arguments, membership file and initialize address book
	int res = init_data(argc, argv);
	if (res) {
		return res;
	}

	//initialize application
	//start listening for incoming UDP packets
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

	// Free data
	free_data();

	return 0;
}
