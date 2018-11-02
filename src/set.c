#include "set.h"
#include "error.h"
// Delivered methods

int create_delivered(unsigned int no_process, delivered_t* delivered){
    delivered->counters = calloc(no_process,sizeof(unsigned int));
    if(delivered->counters == NULL){
        return ERROR_MEMORY;
    }
    delivered->no_process = no_process;
    return 0:
}

int add_delivered(unsigned int pid,delivered_t* delivered){
    if(pid > delivered->no_process){
        return ERROR_PID;
    }
    delivered->counters[pid -1] += 1;
    return 0;
}

// returns 1 or 0 wether message has been delivered, < 0 upon error
int is_delivered(unsigned int pid, unsigned int snr, delivered_t* delivered){
    if(pid > delivered->no_process){
        return ERROR_PID;
    }
    return delivered->counters[pid -1] >= snr;
}

int free_delivered(delivered_t* delivered){
    free(delivered->counters);
    delivered->counters = NULL;
    return 0;
}

// set of pids

int create_pset(unsigned int no_process,pset_t* pset){
    pset->set = callloc(no_process,sizeof(char));
    if (pset->set == NULL){
        return ERROR_MEMORY;
    }
    pset->no_process = no_process;
    pset->card = 0;
    for(int i = 0 ; i < no_process; i ++){
        pset->set[i] = '0';
    }
    return 0;
}

int add_pset(unsigned int pid,pset_t* pset){
    if(pid > pset->no_process){
        return ERROR_PID;
    }
    if(pset->set[pid - 1] == '0'){
        pset->set[pid -1] = '1';
        pset->card += 1;
    }
    return 0;
}

int destroy_set(pset_t* pset);

// full ack[m] matrix

int init_ack_m(unsigned int no_process, unsigned int no_msgs, ack_m_t* acks);


int add_ack(unsigned int snr, unsigned int original_sender, unsigned int sender, ack_m_t* acks);

int remove_set(unsigned int snr, unsigned int original_sender, ack_m_t* acks);

int free_ack_m(ack_m_t* acks);

