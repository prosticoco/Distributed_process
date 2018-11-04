


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// structure for the delivered set
typedef struct{
    unsigned int no_process;
    unsigned int* counters;
}delivered_t;

// structure for the "ack[m]" set
typedef struct{
    unsigned int no_process;
    unsigned int card;
    int* set;
}pset_t;

typedef struct{
    pset_t* sets;
    unsigned int nr_active;
}ack_line_t;

typedef struct{
    unsigned int no_msgs;
    unsigned int no_process;
    ack_line_t* sets;
}ack_m_t;

int create_delivered(unsigned int no_process, delivered_t* delivered){
    delivered->counters = calloc(no_process,sizeof(unsigned int));
    if(delivered->counters == NULL){
        return -1;
    }
    delivered->no_process = no_process;
    return 0;
}

int add_delivered(unsigned int pid,delivered_t* delivered){
    if(pid > delivered->no_process){
        return -1;
    }
    delivered->counters[pid -1] += 1;
    return 0;
}

// returns 1 or 0 wether message has been delivered, < 0 upon error
int is_delivered(unsigned int pid, unsigned int snr, delivered_t* delivered){
    if(pid > delivered->no_process){
        return -1;
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
    pset->set = calloc(no_process,sizeof(int));
    if (pset->set == NULL){
        return -1;
    }
    pset->no_process = no_process;
    pset->card = 0;
    return 0;
}

int add_pset(unsigned int pid,pset_t* pset){
    if(pid > pset->no_process){
        return -1;
    }
    if(pset->set[pid - 1] == 0){
        pset->set[pid -1] = 1;
        pset->card += 1;
    }
    return 0;
}

int destroy_set(pset_t* pset){
    free(pset->set);
    pset->set = NULL;
    pset->card = 0;
    return 0;
}

// full ack[m] matrix

int init_ack_m(unsigned int no_process, unsigned int no_msgs, ack_m_t* acks){
    // only initialize pointers to lines 
    acks->sets = calloc(no_msgs,sizeof(ack_line_t));
    if(acks->sets == NULL){
        return -1;
    }
    acks->no_msgs = no_msgs;
    acks->no_process = no_process;
    return 0;
}



int add_ack(unsigned int snr, unsigned int original_sender, unsigned int sender, ack_m_t* acks){
    // check if sequence number is higher than already allocated array
    while(snr > acks->no_msgs){
        acks->sets = realloc(acks->sets,(2*acks->no_msgs)*sizeof(ack_line_t));
        if(acks->sets == NULL){
            return -1;
        }
        // set the newly allocated region to 0        
        memset(&(acks->sets[acks->no_msgs]),0,acks->no_msgs);
        acks->no_msgs *= 2;
    }
    // allocate a new line if needed
    if(acks->sets[snr-1].nr_active == 0){
        acks->sets[snr-1].sets = calloc(acks->no_process,sizeof(pset_t));
        if(acks->sets[snr -1 ].sets == NULL){
            return -1;
        }
    }
    int error;
    // now create a new set if needed
    pset_t * current_set_ptr = &(acks->sets[snr-1].sets[original_sender-1]);
    if(current_set_ptr->card == 0){
        error = create_pset(acks->no_process,current_set_ptr);
        if(error){
            return error;
        }
        // since new set was created we update the number of active sets
        acks->sets[snr-1].nr_active += 1;
    }
    // now we know the set exists and we can finally add an element :)
    error = add_pset(sender,current_set_ptr);
    if(error){
        return error;
    }
    return 0;
}

int remove_set(unsigned int snr, unsigned int original_sender, ack_m_t* acks){
    int error = destroy_set(&(acks->sets[snr-1].sets[original_sender-1]));
    if(error){
        return error;
    }
    // update number of active sets
    acks->sets[snr-1].nr_active -= 1;
    // if no more sets are active on the line we can free the line
    if(acks->sets[snr-1].nr_active == 0){
        free(acks->sets[snr-1].sets);
        acks->sets[snr-1].sets = NULL;
    }
    return 0;
}

// this method takes care of cleaning all the data at the end of the program
int free_ack_m(ack_m_t* acks){
    int error;
    for(int i = 0 ; i < acks->no_msgs; i++){
        if(acks->sets[i].nr_active > 0){
            for(int j = 0 ; j < acks->no_process; j++){
                if(acks->sets[i].sets[j].card > 0){
                    error = remove_set(i+1,j+1,acks);
                    if(error){
                        return error;
                    }
                }
            }
        }
    }
    free(acks->sets);
    acks->sets = NULL;
    return 0;
}

int main(){
    unsigned int no_process = 4;
    unsigned int no_messages_init = 100;
    ack_m_t matrix;
    int error = init_ack_m(no_process,no_messages_init,&matrix);
    if(error){
        printf("Error while initializing matrix\n");
    }
    printf("Successfully initialized matrix\n");
    return 0;
}






