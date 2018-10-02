//2 functions (send, deliver) which use fair-loss (UDP) and gives perfect Links

//Process needs 2 stacks for this to work, 1 for messages we got (received and delivered)
//And the second to know the acks

//on init these 2 stacks are empty
def send(dest, msg){
  //start by adding flag, assume size is good
  //the flag is hash of dest + message
  //add tupple (of tupple and int) [[dest, hash], 0] to ack table
  //while for key = this tupple the value is 0
      // send udp package trigger <flp2pSend, dest, m ++ hash(m)>
      // sleep
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
