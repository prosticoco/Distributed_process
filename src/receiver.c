
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "receiver.h"
#include "error.h"
#include "mqueue.h"
#include "addrbook.h"
#include "ack.h"
#include "sender.h"


// function callback for receiver threads, each spawned receiver will call this function and start reading msgs
void *receiver_f(void * params) {
    msg_t next_message;
    int error;
    receiver_info_t* data = (receiver_info_t *) params;

    printf("receiver thread, node id : %d \n", data->nodeid);
    printf("my port number is equal to %d \n", data->my_address.sin_port);

    struct sockaddr_in client;
    unsigned int length_client;

    while (1) {
        error = recvfrom(data->fd, (char *) &next_message, sizeof(msg_t),
            MSG_WAITALL, (struct sockaddr *) &client, &length_client);
        if (error < 0) {
            fprintf(stderr,"ERROR : recvfrom return error code %d \n", error);
            exit(ERROR_NETWORK);
        }
        printf("message received\n");

        error = deliver(data, &client, &next_message);
        if (error < 0) {
            fprintf(stderr, "ERROR deliver() error code  %d\n", error);
            exit(error);
        }
    }
    return 0;
}
// creates a socket for receiving messages, updates the value of file descriptor
// returns <0 on error, 0 otherwise
int init_receiver_socket(int* rec_fd, struct sockaddr_in* my_addr) {
    *rec_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*rec_fd < 0) {
        return ERROR_SOCKET;
    }
    if (bind(*rec_fd, (struct sockaddr*) my_addr, sizeof(struct sockaddr_in))) {
        return ERROR_BIND;
    }
    return 0;
}

// spawns receiver thread 
int init_receiver(receiver_info_t* data) {
    int error;
    // initialize the receiver's socket
    error = init_receiver_socket(&(data->fd), &(data->my_address));
    if (error) {
        return error;
    }
    error = pthread_create(data->receiver, NULL, receiver_f, (void*) data);
    if (error) {
        fprintf(stderr,"ERROR pthread_create() return code : %d \n", error);
        return ERROR_THREAD;
    }
    return 0;
}


int end_receiver(receiver_info_t* data) {
    if (close(data->fd)) {
        return ERROR_IO;
    }
    return 0;
}

int deliver(receiver_info_t* data, struct sockaddr_in* sender, msg_t* msg) {
    fprintf(stdout, "DELIVERED CALLED\nReceived message type %d from process %d, message no %d \n",
        msg->msg_type, msg->src_id, msg->msg_nr);
    int error = 0;

    //IF ACK add ack to acklist. basta.
    if (msg->msg_type == ACK_NO) {
        error = add_ack(&(data->acklist), msg->src_id, msg->msg_nr); 
        if (error < 0) {
            return error;
        }
    }
    //real message
    else {
        /**create string from src_id and msg_nr that will be/is written in .out file
         * delivery of application message, using the format
         *   <d sender seq nr>
         * */
        //char* str[128];
        //sprintf(str, "d %u %u", msg->src_id, msg->msg_nr);

        //.out file name for this process
        //char* name[128];
        //sprintf(name, "da proc %u", data->nodeid);

        //check if msg not yet delivered i.e. first time seeing msg
        //if(already_delivered(name, str) == 0){
    //  error = delivered_add(name, str);
            //if(error < 0){
              //  return error;
            //}
        //}

        //create ack message
        // get back address to send ack
        struct sockaddr_in ack_address;
        error = find_addrbook(&(data->addresses), msg->src_id, &ack_address);
        if (error) {
            printf("error searching for id in address book \n");
            return error;
        }
        msg_t ack;
        ack.msg_nr = msg->msg_nr;
        ack.msg_type = ACK_NO;
        ack.src_id = data->nodeid;
        printf("gonna send that bitch an ack for msg no %d \n", msg->msg_nr);
        error = send_fl(data->fd, &ack_address, &ack);
        if (error < 0) {
            printf("wooooooow that did not work because fair loss send fucked up oh shit\n");
            return error;
        }
        fprintf(stdout, "sent ack for message number %d coming from process no %d \n", msg->msg_nr, msg->src_id);
    }
    return 0;
}


/**
int already_delivered(char *fname, char* str){

	FILE *fp;
	int line_num = 1;
	char temp[MAX_MESSAGE_LENGTH];
	
	if((fp = fopen(strcat(fname, ".out"), "r")) == NULL) {
		return ERROR_FILE;
	}

	

	while(fgets(temp, MAX_MESSAGE_LENGTH, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			return 1;
		}
		line_num++;
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return 0;
}

int delivered_add(char *fname, char* str){

    int error;
    FILE *fp;

    if(fp = fopen(strcat(fname, ".out"), "a") == NULL ) {
        return ERROR_FILE;
    }
    error = fprintf(fp, str);
    if(error < 0){
        return ERROR_FILE;
    }

    if(fp) {
		fclose(fp);
	}
   	return 0;

}
*/

