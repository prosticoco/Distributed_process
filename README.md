# DA-Project
Distributed Algorithms Project

## Things to do

* Find out how to send UDP messages with C
* Find out how to start a process with C
* Implement perfect links _directly_ over fair-loss links, do not implement stubborn links as is done in the class
* Implement Broadcasts using perfect links when we had a REAL course about broadcasts

##Ideas

* When sending a message, add pair [dest, 0] to ack table and change 0 to 1 when receive ack from that dest
        //this way we can easily keep track
* In each process have a thread for each dest node so that they can advance through multiple messages asynchronously
* We can never assume a process is ded. 
        //By theory in an asynchronous system it is impossible to know if a process is sleeping or has crashed

