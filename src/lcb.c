
#include "data.h"
#include "layers.h"
#include "pending.h"
#include "dependencies.h"





int update_vec_clock(net_data_t* data, unsigned int pid){
    if(pid > data->num_proc){
        return ERROR_PID;
    }
    data->vector_clock->vector[pid-1] += 1;
    return 0;
}



int test_vec_clock(net_data_t* data,vec_clock_t* vector,unsigned int pid){
    dependency_list_t* dep = get_dependencies(data->dependency_matrix,pid);
    int is_bigger = 1;
    for(int i = 0; i < dep->list_len; i++){
        unsigned int curr_proc = dep->pid_list[i];
        if(vector->vector[curr_proc -1] > data->vector_clock->vector[curr_proc -1]){
            is_bigger = 0;
        }
    }
    return is_bigger;
}

int find_pending(net_data_t* data){


}