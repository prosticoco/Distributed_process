#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "data.h"
#include "dependencies.h"
#include "error.h"
#include "fifo.h"
#include "causal.h"
#include "layers.h"
#include "log.h"
#include "mqueue.h"
#include "parser.h"
#include "plink.h"
#include "receiver.h"
#include "sender.h"
#include "urb.h"
#include "pending.h"

#define QUEUE_LEN 200
#define NUM_SENDER_THREADS 10
#define MSG_BUF_MAX_SIZE 1024
#define LOG_FILENAME "da_proc_%zu.out"
#define ORIG_TABLE_SIZE 150

/* ----- GLOBAL DATA ----- */
msg_queue_t main_queue;
ack_table_t ack_table;
pl_delivered_t delivered_pl;
urb_table_t urb_table;
pending_t pending_table;
next_t next_table;
log_data_t log_data;
urb_ack_table_t urbacks;
lcb_pending_t lcb_pending;
vec_clock_t vec_clock;
char filename[14];


net_data_t net_data = {
	.address_book = NULL,
	.num_msg = 0,
	.num_proc = 0,
	.self_pid = 0,
	.senders = NULL,
	.pl_acks = &ack_table,
	.pl_delivered = &delivered_pl,
	.pending = &pending_table,
	.next = &next_table,
	.urb_table = &urb_table,
	.task_q = &main_queue,
	.logdata = &log_data,
	.urbacks = &urbacks,
	.lcb_pending = &lcb_pending,
	.vector_clock = &vec_clock
};

static int wait_for_start = 1;


/* ----- UTILITY FUNCTIONS ----- */

static int init_data(int argc, char** argv) {
	int res = parse_membership_args(argc, argv, &net_data);
	if (res) {
		return res;
	}
	res = init_queue(net_data.task_q, QUEUE_LEN);
	if (res) {
		return res;
	}
	res = init_ack_table(net_data.pl_acks, ORIG_TABLE_SIZE, net_data.num_proc);
	if (res) {
		return res;
	}
	res = init_deliver_pl(net_data.pl_delivered, ORIG_TABLE_SIZE, net_data.num_proc);
	if (res) {
		return res;
	}
	res = init_urb_table(net_data.urb_table, ORIG_TABLE_SIZE, net_data.num_proc);
	if (res) {
		return res;
	}
	res = init_ack_urb(net_data.urbacks,ORIG_TABLE_SIZE,net_data.num_proc);
	if(res){
		return res;
	}
	/**
	 * FIFO LAYER METHODS, UNCOMMENT TO USE FIFO INSTEAD OF LCB
	 * 
	 */
	/*
	res = init_pending(net_data.pending, ORIG_TABLE_SIZE, net_data.num_proc);
	// TODO: change that call the one for LCB instead of FIFO B
	res += init_next(net_data.next, net_data.num_proc);
	if (res) {
		return res;
	}
	*/
	res = init_pending_lcb(net_data.lcb_pending,net_data.num_proc);
	if(res < 0){
		printf("Error initializing pending lcb \n");
		return res;
	}
	res = init_vector_mutex(net_data.vector_clock,net_data.num_proc);
	if(res < 0){
		printf("Error initializing vector clock \n");
		return res;
	}
	res = sprintf(net_data.log_filename, LOG_FILENAME, net_data.self_pid);
	if (res < 0) {
		return res;
	}
	fflush(stdout);
	res = init_log_data(net_data.logdata, MSG_BUF_MAX_SIZE, net_data.log_filename);
	if (res) {
		return res;
	}

	res = init_receiver(&net_data);
	if (res) {
		return res;
	}

	res = init_senders(&net_data, NUM_SENDER_THREADS);
	if (res) {
		return res;
	}
	
	return res;
}

static void free_data(void) {
	free_addr_book(net_data.address_book);
	free_dependencies(net_data.dependencies);
	//free_dependencies(net_data.reverse_dependencies);
	free_queue_and_vectors(net_data.task_q);
	free_ack_table(net_data.pl_acks);
	free_urb_table(net_data.urb_table);
	// TODO: change that call the one for LCB instead of FIFO B
	//free_pending(net_data.pending);
	//free_next(net_data.next);
	
	free_log_data(net_data.logdata);
	free_delivered(net_data.pl_delivered);
	free_ack_urb(net_data.urbacks);	
	destroy_pending_lcb(net_data.lcb_pending);
	destroy_vector(net_data.vector_clock);
}

static void start(int signum) {
	wait_for_start = 0;
}

static void stop(int signum) {
	//reset signal handlers to default
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	//immediately stop network packet processing
	terminate_senders(&net_data, NUM_SENDER_THREADS);
	terminate_receiver(&net_data);

	//write/flush output file if necessary
	write_to_file(net_data.logdata);

	// Free resources.
	free_data();

	//exit directly from signal handler
	exit(0);
}


/* ----- MAIN ----- */

int main(int argc, char** argv) {
	// Set signal handlers
	signal(SIGUSR2, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);

	// Starts threads, but threads wait before sending messages.
	int res = init_data(argc, argv);
	if (res) {
		free_data();
		return res;
	}

	// Wait until start signal
	while (wait_for_start) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 0;
		sleep_time.tv_nsec = 1000;
		nanosleep(&sleep_time, NULL);
	}

	// Broadcast messages
	printf("Broadcasting messages...\n");
	res = send_lcb(&net_data, net_data.num_msg);
	if (res) {
		printf("Failed Localized Causal Broadcast.\n");
		return res;
	}
	// Wait until stopped
	while (1) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 1;
		sleep_time.tv_nsec = 0;
		nanosleep(&sleep_time, NULL);
	}
}
