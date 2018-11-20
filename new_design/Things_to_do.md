#Localized Causal Broadcast for December 16th 2018

For the 2nd part of the project we need to implement Localized Causal Broadcast

##Description
1) The extra params command-line argument for this application consists
of an integer m, which defines how many messages each process should
broadcast. (same as for FIFO)

2) The application-dependent data at the end of the membership file for this
application consists of n lines. Each line i corresponds to process i, and
such a line indicates the identities of other processes which can affect
process i. See the example below.

3) In the output log for this application, we require that each line
marking the delivery of a messages also logs the set of depen-
dencies for that message. This set should only include the 1-hop de-
pendencies.

##TO DO

- change Membership file parser so that it takes in the n additional lines
that describe the network layout

ex:

5
1 127.0.0.1 11001
2 127.0.0.1 11002
3 127.0.0.1 11003
4 127.0.0.1 11004
5 127.0.0.1 11005
1 4 5                 (process 1 is affected only by messages broadcast by 4 and 5)
2 1
3 1 2
4 1 2
5 3 4

process x is affected by process z if all messages which process z broadcasts and which x delivers become dependencies for process x

- change log function so that we log message dependencies 

- implement localised causal broadcast

## Theory

local order causality: some process pi delivers m1 and then broadcasts m2

in our application this means that process 1 broadcasts his messages depending on the messages delivered from processes 4 and 5

(if message_x_from_4 delivered and message_y_from_5 delivered):
    broadcast(message_z_from_1) where z is dependant on x and y

We can see FIFO broadcast as localized causal broadcast where for all processes only the process itself is a dependancy for that process
1 1
2 2
3 3 etc...

so by intuition to implement Local Causal, we can just use FIFO deliver (to enforce message from same process order) and just modify FIFO_broadcast to only send messages when the causality condition is met:

LCB:

on init:
    per process create table/variable for messages received from depending processes called past
ex: process 1 has 2 past tables 1 for 4 and 1 for 5
it saves seq_nr of past message(s) received from the process in question

LCB_send(m):
    msg 0 can always be send
    for msg in range(1,m):
        if(past_4.contains(seq_nr(msg)-1) && past_5.contains(seq_nr(msg)-1)):
            urb_send(msg)
        else:
            wait()
LCB_deliver(msg):
    same as fifo just if sender_id is in locality, add seq_nr(msg) to table

