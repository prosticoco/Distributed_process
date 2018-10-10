
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


// Program create 2 threads, 1 writer threads and 1 reader thread,
// writer thread sends a message to reader thread

static int ready_to_connect = 0;

// CODE to send messages via udp
void *writer1f(void *params){
    // this structure represents the address
    struct sockaddr_in read_addr;
    // integer representing the file descriptor of the socket
    int socketfdw1;
    // create a socket (flags to indicate using ip address and UDP Datagrams)
    socketfdw1 = socket(AF_INET, SOCK_DGRAM,0);
    if(socketfdw1 < 0){
      printf("socket opening did not work\n");
      return NULL;
    }

    // initialize the address field
    bzero((char*) &read_addr, sizeof(read_addr));

      // same for any address
    read_addr.sin_family = AF_INET;
    // portno converted from integer using htons() function
    read_addr.sin_port = htons(11001);
    // converted IP Address String using inet_aton() function
    inet_aton("127.0.0.1", (struct in_addr *) &read_addr.sin_addr.s_addr);


    // don't need to do this
    while(!ready_to_connect){
      sleep(1);
    }

    // text to send
    char buffer[] = "Hello I am writer no 1";
    int n;
    // send the message
    n = sendto(socketfdw1, (const char *) buffer,strlen(buffer),MSG_CONFIRM,
  (const struct sockaddr *) &read_addr, sizeof(read_addr));
    if(n < 0){
      printf("not able to write \n");
      return NULL;
    }
}


void *readerf(void *params){
  
  int sockr,clilen,newsockfd;
  char buffer[256];
  struct sockaddr_in my_addr,cli_addr;
  int n;
  sockr = socket(AF_INET,SOCK_DGRAM,0);
  if(sockr < 0){
    printf("socket did not work\n");
    return NULL;
  }
  bzero((char *) &my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  inet_aton("127.0.0.1", (struct in_addr *) &my_addr.sin_addr.s_addr);
  my_addr.sin_port = htons(11001);
  if(bind(sockr, (struct sockaddr *) &my_addr,sizeof(my_addr)) < 0){
    printf("Error binding \n");
    return NULL;
  }
  ready_to_connect = 1;
  int len;
  n = recvfrom(sockr, (char *) buffer,255,
  MSG_WAITALL, (struct sockaddr *) &cli_addr,&len);
  buffer[n] = '\0';
  printf("MESSAGE RECIEVED ON READER\n");
  printf("%s\n",buffer);
}



int main(){
    int x = 0;
    pthread_t writer1;
    pthread_t reader;
    if(pthread_create(&reader,NULL, readerf, &x)){
      printf("error creating reader thread \n");
      return -1;
    }

    if(pthread_create(&writer1,NULL,writer1f,&x)){
      printf("error creating writer thread\n");
      return -1;
    }

    if(pthread_join(reader,NULL)){
      printf("error joining reader thread\n");
      return -1;
    }

    if(pthread_join(writer1,NULL)){
      printf("error joining writer1 thread\n");
      return -1;
    }
    printf("threads completed successfully\n");
    return 0;


}
