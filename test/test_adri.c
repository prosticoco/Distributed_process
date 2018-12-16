#include "layerzero.h"
#include "pending.h"
#include "data.h"


#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h>


int test_serial(){
    printf("---- TEST SERIALISATION ---- \n");
    msg_t msg0;
    unsigned int clock0[] = {1,2,3,4,5};
    msg0.ackid = 0;
    msg0.mid = 10;
    msg0.mtype = 20;
    msg0.sender = 30;
    msg0.urb_msg.mid = 40;
    msg0.urb_msg.seen_id = 50;
    int error = alloc_vector(&(msg0.urb_msg.lcb_msg.vec_clock),5,clock0);
    if(error){
        printf("alloc vector error \n");
        return error;
    }
    error = print_message(msg0);
    char* buffer;
    error = serialize(&msg0,&buffer,5);
    if(error){
        printf("error in serialize \n");
        return error;
    }
    msg_t msg1;
    error = deserialize(&msg1,buffer,5);
    if(error){
        printf("error in deserialize \n");
        return error;
    }
    print_message(msg1);
    return 0;
}

int send_test(struct sockaddr_in address,int socket_fd,msg_t msg){
    int error;
    char* buffer;
    error = serialize(&msg,&buffer,msg.urb_msg.lcb_msg.vec_clock.num_proc);
    if(error){
        printf("Serialization error \n");
        return error;
    }
    error = sendto(socket_fd,(const char *) buffer,
                   message_size(msg.urb_msg.lcb_msg.vec_clock.num_proc),MSG_DONTWAIT,
                   (const struct sockaddr*) &address,
                   sizeof(struct sockaddr_in));
    if(error < 0){
        printf("Error sending \n");
        return ERROR_SEND;
    }
    printf("Sent message ;-) \n");
    if(buffer != NULL){
        free(buffer);
    }
    error = destroy_vector(&(msg.urb_msg.lcb_msg.vec_clock));
    if(error){
        printf("Destroying temporary vector clock failed \n");
        return error;
    }
    return 0;
}

int receive_test(int receiver_fd,char* buffer, msg_t* next_message, size_t num_proc){
    int error;
    struct sockaddr_in client;
    unsigned int length_client;
    size_t msg_size = message_size(num_proc);
    if(buffer == NULL){
        printf("Buffer passed to receive_msg is null \n");
        return ERROR_POINTER;
    }
    error = recvfrom(receiver_fd,(char*) buffer,msg_size,
                     MSG_WAITALL,(struct sockaddr *) &client,&length_client);
    if(error < 0){
        printf("message reception error  \n");
        return ERROR_NETWORK;
    }
    error = deserialize(next_message,buffer,num_proc);
    if(error){
        printf("Deserialization error \n");
        return error;
    }
    return 0;
}

int server_setup(int port){
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("Socket creation failed \n");
        return ERROR_SOCKET;
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 )
        {
            printf("Binding failed \n");
            return ERROR_BIND;
        }
    return sockfd;
}

int client_setup(int port,struct sockaddr_in* serv){
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0){
        printf("clisetup :Socket creation failed\n");
        return ERROR_SOCKET;
    }
    memset(serv,0,sizeof(*serv));
    serv->sin_family = AF_INET;
    serv->sin_port = htons(port);
    serv->sin_addr.s_addr = inet_addr("127.0.0.1");
    return sockfd;
}

int sertestsender(){
    int fd;
    struct sockaddr_in address;
    fd = client_setup(11001,&address);
    if(fd < 0){
        printf("error setup client \n");
        return fd;
    }
    msg_t msg;
    unsigned int clocki[] = {5,4,3,2,1};
    msg.ackid = 0;
    msg.mid = 420;
    msg.mtype = 1;
    msg.sender = 69;
    msg.urb_msg.mid = 46;
    msg.urb_msg.seen_id = 1152;
    int error = alloc_vector(&(msg.urb_msg.lcb_msg.vec_clock),5,clocki);
    if(error){
        printf("allocation of vector did not work \n");
        return error;
    }
    print_message(msg);
    error = send_test(address,fd,msg);
    if(error){
        printf("send test error \n");
        return error;
    }
    return 0;
}

int sertestreceiver(){
    int fd;
    fd = server_setup(11001);
    if(fd < 0){
        printf("server setup did not work \n");
        return fd;
    }
    char buffer[message_size(5)];
    memset(buffer,0,message_size(5));
    msg_t next;
    int error = receive_test(fd,buffer,&next,5);
    if(error){
        printf("error with receive \n");
        return error;
    }
    printf("received message : \n");
    print_message(next);
    error = destroy_vector(&(next.urb_msg.lcb_msg.vec_clock));
    if(error){
        printf("destroy error \n");
        return error;
    }
    return 0;
}

void *thread_receive_test(void * args){
    int error = sertestreceiver();
    if(error){
        printf("error sertestreceiver\n");
        exit(error);
    }
    return 0;
}

void *thread_send_test(void * args){
    int error = sertestsender();
    if(error){
        printf("error sertestsender \n");
        exit(error);

    }
    return 0;

}

int serial_full_test(){
    int error;
    int dump;
    pthread_t sender;
    pthread_t receiver;
    error = pthread_create(&sender,NULL,thread_send_test,&dump);
    if(error){
        printf("error creating teddy send \n");
        return error;
    }
    error = pthread_create(&receiver,NULL,thread_receive_test,&dump);
    if(error){
        printf("error creating teddy receiver \n");
        return error;
    }
    pthread_join(sender,NULL);
    pthread_join(receiver,NULL);
    printf("threads joined \n");
    return 0;
}

int main(){

    int error = test_serial();
    if(error){
        printf("error in test_serial \n");
        return error;
    }
    error = serial_full_test();
    if(error){
        printf("error serial full tesr \n");
        return error;
    }
    return 0;
}