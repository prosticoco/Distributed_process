#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "data.h"
#include "mqueue.h"
#include "parser.h"
#include "plink.h"

/* ----- GLOBAL DATA ----- */
net_data_t net_data = {
	.address_book = NULL,
	.num_msg = 0,
	.self_pid = 0,
	.pl_acks = NULL,
	.task_q = NULL
};

static int wait_for_start = 1;



static int init_data(int argc, char** argv) {
	printf("Parsing arguments, membership file and filling address book...\n");
	int res = parse_membership_args(argc, argv, &net_data);
	if (res) {
		return res;
	}
	printf("Initializing message queue...\n");
	// TODO: do something
	res = init_queue(net_data.task_q, 2000);
	if (res) {
		return res;
	}
	printf("Initializing ack table...\n");
	res = init_ack_table(net_data.pl_acks, net_data.num_msg, net_data.num_proc);
	if (res) {
		return res;
	}
	printf("Initializing sockets...\n");
	// TODO
	return res;
}

static void free_data(void) {
	free_addr_book(net_data.address_book);
	free_queue(net_data.task_q);
	free_ack_table(net_data.pl_acks);
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

	// Set signal handlers
	signal(SIGUSR2, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);

	// 1. Parse arguments, membership file and initialize data structures
	int res = init_data(argc, argv);
	if (res) {
		return res;
	}




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

	// Free data
	free_data();

	return 0;
}
