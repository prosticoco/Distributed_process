#include <stdlib.h>

// structure for the delivered set
typedef struct{
    unsigned int no_process;
    unsigned int* counters;
}delivered_t;

// structure for the "ack[m]" set
typedef struct{
    unsigned int no_process;
    unsigned int card;
    char* set;
}pset_t;


typedef struct{
    unsigned int no_msgs;
    unsigned int no_process;
    pset_t** sets;
}ack_m_t;

// Delivered methods

int create_delivered(unsigned int no_process, delivered_t* delivered);

int add_delivered(unsigned int pid,delivered_t* delivered);

int is_delivered(unsigned int pid, unsigned int snr, delivered_t* delivered);

int free_delivered(delivered_t* delivered);

// set of pids

int create_pset(unsigned int no_process,pset_t* pset);

int add_pset(unsigned int pid,pset_t* pset);

int destroy_set(pset_t* pset);

// full ack[m] matrix

int init_ack_m(unsigned int no_process, unsigned int no_msgs, ack_m_t* acks);


int add_ack(unsigned int snr, unsigned int original_sender, unsigned int sender, ack_m_t* acks);

int candeliver(unsigned int snr, unsigned int original_sender, ack_m_t* acks);

int remove_set(unsigned int snr, unsigned int original_sender, ack_m_t* acks);

int free_ack_m(ack_m_t* acks);

