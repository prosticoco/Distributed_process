# DA-Project
Distributed Algorithms Project

## Things to do

* Find out how to send UDP messages with C
* Find out how to start a process with C
* Implement perfect links _directly_ over fair-loss links, do not implement stubborn links as is done in the class

## Program flow proposition

Setup : Multithreaded program :
  - Process represents 1 node in a graph of connected nodes which communicate with each other

  - will consist of one main thread responsible of initialization and signal handling
  - n other threads responsible for sending msgs to other nodes (sender threads)
  - 1 more thread responsible of processing msgs from other nodes (receiver thread)

  Main thread :
  - reads the membership file and initializes arrays that will be used as shared memory with other threads
  - handles incoming signals such as USR1/TERM/int
  USR1 : communicates to the sender threads using shared memory that they can start sending msgs

  TERM/INT :orders other threads to terminate their work ,fetches the data recorded by other threads (data about which messages were successfully sent etc) and writes it to the log file, and then exits the program

  Sender Threads :

  each sender thread is assigned one specific node to send msgs to :
    it reads the data from the shared memory in order to know which message it has to send (either ordinary msg or ACK) and then sends it, then repeats doing so
    Each sender thread can be considered as a client of an other node, each messages sent by a sender thread will use perfect link. The idea is to code a method send(args) which will be called by the thread for each msg it has to send and will not return until the message is successfully sent


  Receiver Thread :

  This thread acts as the server of the node. Each time it receives a message, it updates the shared data accordingly and then goes back to waiting for other messages


  This is the main idea for the program structure, a lot can be changes such as new threads can be added if needed.


  Shared Memory :

  Idea is to have arrays that will be updated by the receiver thread and read by the sender threads, these arrays should contain information such as :

  - - what messages still have to be sent by each thread
  - - for each other node which ACK have already been received etc..
  - - signal information
