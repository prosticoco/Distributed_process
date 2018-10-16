#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "addrbook.h"
#include "ack.h"
#include "mqueue.h"
#include "error.h"
#include "utils.h"
#include "sender.h"
#include "receiver.h"


// new condition variable to wake up threads for them to start sending messages
static pthread_cond_t start_condition = PTHREAD_COND_INITIALIZER;
// mutex used with the condition variable
static pthread_mutex_t start_mutex = PTHREAD_MUTEX_INITIALIZER;

static receiver_info_t data;


int test_init(receiver_info_t* data, unsigned int node_num,unsigned int total_nodes){
	da_process_t other_processes[total_nodes-1];
	data->nodeid = node_num;
	data->no_nodes = total_nodes -1;
	switch(node_num){

		case 1 :
			if(!inet_aton("127.0.0.1", &(other_processes[0].ipv4_addr) )){
				fprintf(stderr,"ERROR inet_aton");
				return ERROR_MEMORY;
			}
			other_processes[0].uid = 2;
			other_processes[0].port_num = htons(11002);
			init_addrbook(data->addresses,total_nodes-1,other_processes);
			data->my_address->sin_family = AF_INET;
			if(!inet_aton("127.0.0.1", &(data->my_address->sin_addr))){
				fprintf(stderr,"ERROR inet_aton");
				return ERROR_MEMORY;
			}
			data->my_address->sin_port = htons(11001);
			init_acks(data->acklist,total_nodes -1 , other_processes);
		case 2 :
			if(!inet_aton("127.0.0.1", &(other_processes[0].ipv4_addr) )){
				fprintf(stderr,"ERROR inet_aton");
				return ERROR_MEMORY;
			}
			other_processes[0].uid = 1;
			other_processes[0].port_num = htons(11001);
			init_addrbook(data->addresses,total_nodes-1,other_processes);
			data->my_address->sin_family = AF_INET;
			if(!inet_aton("127.0.0.1", &(data->my_address->sin_addr))){
				fprintf(stderr,"ERROR inet_aton");
				return ERROR_MEMORY;
			}
			data->my_address->sin_port = htons(11002);
			init_acks(data->acklist,total_nodes -1 , other_processes);
	}
	return 0;
}

void end_test(receiver_info_t* data){
	free_addrbook(data->addresses);
	free_acks(data->acklist);
}


static void free_resources(void) {
	if (data.sender_infos != NULL) {
		terminate_senders(&data);
	}
	end_receiver(&data);
	end_test(&data);
}

// function called when process recieves start signal
// process will broadcast messages to other processes
static void start(int signum) {
	fprintf(stdout,"START SENDING MESSAGES\n");
	int error = 0; 
	error = pthread_mutex_lock(&start_mutex);
	if(error){
		fprintf(stderr,"ERROR pthread_mutex_lock in start() error code %d\n",error);
		return;
	}
	error = pthread_cond_broadcast(&start_condition);
	if(error){
		fprintf(stderr,"ERROR pthread_cond_broadcast error code %d\n",error);
	}
	error = pthread_mutex_unlock(&start_mutex);
	if(error){
		fprintf(stderr,"ERROR pthread_mutex_unlock in start() error code %d\n",error);
	}

}

static void stop(int signum) {
	// Reset signal handlers to default
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);


	// Immediately stop network packet processing
	printf("Immediately stopping network packet processing.\n"
	"killing all other fucking threads \n");
	pthread_kill_other_threads_np();
	// Write/flush output file if necessary
	printf("Freeing the fucking stupid ressources\n");
	free_resources();
	printf("Writing output. lol just joking we did not implement this shit yet\n");

	// Free resources
	

	// Exit directly from signal handler
	exit(0);
}

int main(int argc, char** argv) {
	int error;
	// IMPORTANT HAVE TO FILL IN ALL OF THE FIELDS IN DATA BEFORE INITIALIZING THREADS
	// Except filedescriptor field
	// lol I mean fill in any data you can
	// need parsing of membership file to get the number of nodes
	// default values 
	// initalize values for testing, hardcoded in this method
	test_init(&data,atoi(argv[1]), 2);
	// Initialize values with membership file.
	//parse_membership_args(argc, argv, &data);
	// initialize list of pthreads
	pthread_t sender_threads[data.no_nodes];
	// receiver thread
	pthread_t receiver_thread;

	// fields that do not require membership file to be initialized
	data.senders = sender_threads;
	data.receiver = &receiver_thread;
	data.start = &start_condition;
	data.start_m = &start_mutex;

	// TODO: maybe move this after all the data has been initialized ?
	// Set signal handlers
	signal(SIGUSR1, start);
	signal(SIGTERM, stop);
	signal(SIGINT, stop);

	printf("MT : Finished initializing values of data\n");

	error = init_receiver(&data);
	if(error){
		fprintf(stderr,"MT : Error Initializing receiver error code %d \n",error);
		return error;
	}
	printf("MT : finished initializing receiver thread with success\n");

	error = init_senders(&data);
	if(error){
		fprintf(stderr,"MT : Error Initializing sender threads, error code %d\n",error);
		return error;
	}
	printf("MT : finished initializing sender threads with success\n");

	printf("MT : Now gonna sleep and wait for my signals hihi \n");
	while (1) {
		struct timespec sleep_time;
		sleep_time.tv_sec = 1;
		sleep_time.tv_nsec = 0;
		nanosleep(&sleep_time, NULL);
	}
}
