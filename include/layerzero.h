
#include "data.h"
#include "error.h"
#include "pending.h"


int serialize(msg_t* msg, char** buffer,size_t num_proc);

int deserialize(msg_t * msg,char* buffer,size_t num_proc);

int print_message(msg_t msg);