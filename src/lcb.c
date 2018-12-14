
#include "data.h"
#include "layers.h"
#include "pending.h"


int update_vec_clock(net_data_t* data, unsigned int pid){
    if(pid > data->num_proc){
        return ERROR_PID;
    }
    data->vector_clock->vector[pid-1] += 1;
    return 0;
}

int test_vec_clock(net_data_t* data,vec_clock_t* vector,unsigned int pid){
    
}