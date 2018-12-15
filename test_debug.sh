#!/bin/bash
#
# Tests the correctness of the Uniform Reliable Broadcast application.
#
# This is an example script that shows the general structure of the
# test. The details and parameters of the actual test might differ.
#

# time to wait for correct processes to broadcast all messages (in seconds)
# (should be adapted to the number of messages to send)
time_to_finish=2

init_time=4

# configure lossy network simulation
sudo tc qdisc change dev lo root netem delay 50ms 200ms loss 10% 25% reorder 25% 50%

# compile (should output: da_proc)
make

echo "2
1 127.0.0.1 11001
2 127.0.0.1 11002
1 2
2" > membership

# start 5 processes, each broadcasting 100 messages
for i in `seq 1 2`
do
    ./da_proc $i membership 2 &
    da_proc_id[$i]=$!
done

# leave some time for process initialization
sleep $init_time



# start broadcasting
for i in `seq 1 2`
do
    if [ -n "${da_proc_id[$i]}" ]; then
	kill -USR2 "${da_proc_id[$i]}"
    fi
done



# leave some time for the correct processes to broadcast all messages

sleep $time_to_finish



# stop all processes
for i in `seq 1 2`
do
    if [ -n "${da_proc_id[$i]}" ]; then
    echo "killing process i"
	kill -TERM "${da_proc_id[$i]}"
    fi
done

# wait until all processes stop
for i in `seq 1 2`
do
    if [ -n "${da_proc_id[$i]}" ]; then
	    wait "${da_proc_id[$i]}"
    fi
done

