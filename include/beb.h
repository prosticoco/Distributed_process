#pragma once
//best effort broadcast

//when asked to broadcast message m, need to create message for every process in addr book 
//and add this task to the queue

int beb_send(beb_data_t* data, msg_t* msg)

int beb_deliver(beb_data_t* data, msg_t* msg)