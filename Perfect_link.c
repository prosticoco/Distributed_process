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


//2 functions (send, deliver) which use fair-loss (UDP) and gives perfect Links

//Process needs 2 stacks for this to work, 1 for messages we got (received and delivered)
//And the second to know the acks

//on init these 2 stacks are empty
void send(struct sockaddr_in* dest, uint dest_nr, Message* msg, bool*** acks){


    int socketfdw1;
    // create a socket (flags to indicate using ip address and UDP Datagrams)
    socketfdw1 = socket(AF_INET, SOCK_DGRAM,0);
    if(socketfdw1 < 0){
      printf("socket opening did not work\n");
      return NULL;
    }


    int n;

    //tupple (uint and int) [msg_nr, 0] already added to ack table
    //while for key = this tupple the value is 0
    //we cant copy the ack table as tmp since we wait for it to change by the other thread
    while(*acks[dest_nr][msg->msg_nr] == 0){

      // send the message, instead of saying buffer = mtext, just put mtext directly in place of buffer
      n = sendto(socketfdw1, (const char *) msg.mtext ,strlen(msg -> mtext),MSG_CONFIRM,
      (const struct sockaddr *) &read_addr, sizeof(read_addr));
      if(n < 0){
        printf("not able to write \n");
        return NULL;
      }
      // sleep
      sleep(10);
    }
    //repeat until got acks
}

def deliver(src, msg){
  //check if msg is ack
  //if ack
  //find same key and set value from 0 to 1

  //else not ack => its a message
  //if hash(me+message) == hash received
  // ==> it was destined for me
  //if delivered contains message ==> my ack was lost ==> resend acks
  //if delivered does not contain message
  //trigger <pp2pDeliver, src, m>
  //add message to delivered
  send(src, [[me, hash], 1]) //<<<<< ack
}


typedef struct{
  size_t msg_type; //if ack or not
  uint msg_nr;
  uint srcId;
  char mtext[256];
}Message;
