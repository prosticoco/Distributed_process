#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
<<<<<<< Updated upstream
#include <string.h>

#include "da_proc.h"
#include "utils.h"
=======
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MESSAGE_LENGTH 256

typedef struct{
		size_t msg_type;
		int srcId;
		char mtext[MESSAGE_LENGTH];
}message;
>>>>>>> Stashed changes

static int wait_for_start = 1;
static int total_process_number = 0;
static struct da_process this_process;

// function called when process recieves start signal
// process will broadcast messages to other processes
static void start(int signum) {
	wait_for_start = 0;
	//setup client socket
	int cl_socketfd;
	// declare a struct which represent the address of the other processes
	struct sockaddr_in proc_addr;
	// portno will have to be affiliated to the corresponding portnumbers in
	// the membership file
	int portno;
+
	// create a udp socket for the client side
	cl_socketfd = socket(AF_INET,SOCK_DGRAM,0);
	// reset the address structure
	bzero(char*) &proc_addr, sizeof(proc_addr));
	proc_addr.sin_family = AF_INET;
	size_t num_proc = 0 ;

	// prepare the message to be sent



	// iterate on all the processes and broadcast the message

	for(int i = 0 ; i < num_proc; i++){

	}

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
	int res = parse_membership_args(argc, argv, &total_process_number, &this_process);
	if (res != 0) {
		return res;
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
