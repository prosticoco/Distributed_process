# New idea for project redesign :

## Parameters : 

- Parsed from membership file
- N Processes with their address/port and unique ID (Sequenced from 1 to N)
- ID of current process from {1,...,n}

## Thread Structure :

- N-1 Threads (Senders) only take care of sending messages through perfect link (they each wait on a personal queue of messages)

- One thread (Receiver) reads from the incoming messages from a socket and adds them to a queue to be processed.

- One thread (Controller) waits on the queue of msgs to process and takes care of processing messages.

## Data Structures :

**Network Data**

- We will use one main struct that will contain pointers to every list/queues/matrices structures needed by any thread

**Queue** 

- We implement a circular queue that can be mutex locked if needed 
_TODO_
- Still need to think about the maximum lenght of the queue and how to handle the cases
when blocked threads have a full queue (maybe ignore, but for security use big enough queue, maybe according to no of other processes)

- Queue will be used in the following contexts :

    -  (Sender Queues) for message sending (Controller writes to queue, Senders read from queue) _Mutex Locked_
    -  (Deliver Queue) for message processing (Receiver writes to queue, Controller reads from queue) _Mutex Locked_

**URB Matrix**

_ACCESSED ONLY BY CONTROLLER THREAD_

- We implement the pending/ack[m] scheme together in a single N x N Matrix

    - [ROWS] i-th row represents messages sent by process i+1
    - [COLUMNS] j-th column represents messages received by process j+1
    - [ENTRIES] Integer corresponding to message sequence number
    
    * In other words, if M[i,j] == 2, that means that process j+1 has seen the message of sequence #2 originally sent by process i+1

**Perfect Link ACK**

_TODO_
- Need to think of most efficient way to share any ACK knowledge with Sender Threads

## Synchronisation

- As mentionned above, the queues will be mutex locked to ensure synchronisation between the controller and the senders
- As done in the previous version, we will first spawn all threads including senders, these bitches (senders) will wait on a condition mutex to start sending
- Only the sender threads will wait on this conditions, which ensures that we can already process messages before application starts


## Pseudo Codes :

