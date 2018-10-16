#include "ack.h"
#include "error.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>



// lock to use on matrix
static pthread_rwlock_t ack_lock;
static pthread_rwlockattr_t attribute;
static int lock_in_use = 0;


static ack_counter_t* get_ack_counter(ack_data_t* acks, unsigned int pid) {
    // TODO: might be possible to do it in a more efficient way
    // Find ack counter bound to given pid
    ack_counter_t* this_counter = NULL;
    for (size_t i = 0; i < acks->size; ++i) {
        ack_counter_t* counter = acks->acks + i;
        if (counter->pid == pid) {
            this_counter = counter;
            break;
        }
    }

    return this_counter;
}


// only call once initializes acks structure and initializes a reader/writer lock
int init_acks(ack_data_t* data,size_t num_proc,da_process_t* proc_list){
    int error;
    if (lock_in_use) {
        return ERROR_ACK_LOCK;
    }
    error = pthread_rwlock_init(&ack_lock,&attribute);
    if (error) {
        return ERROR_LOCK;
    }
    error = pthread_rwlockattr_setkind_np(&attribute, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    if (error) {
        return ERROR_LOCK;
    }
    lock_in_use = 1;
    data->acks = calloc(num_proc,sizeof(ack_counter_t));
    if (data->acks == NULL) {
        return ERROR_MEMORY;
    }
    for (int i = 0 ; i < num_proc; i++) {
        data->acks[i].counter = 0;
        data->acks[i].pid = proc_list[i].uid;
    }
    return 0;
}


int add_ack(ack_data_t* acks, unsigned int pid, unsigned int msg_no) {
    // Acquire lock for writing
    if (pthread_rwlock_wrlock(&ack_lock)) {
        return ERROR_LOCK;
    }

    // Find ack counter bound to given pid
    ack_counter_t* this_counter = get_ack_counter(acks, pid);
    if (this_counter == NULL) {
        return ERROR_PID;
    }

    // Increment counter only if given message isn't acked already
    if (this_counter->counter < msg_no) {
        this_counter->counter++;
    }

    // Release lock
    if (pthread_rwlock_unlock(&ack_lock)) {
        return ERROR_LOCK;
    }

    return 0;
}

int read_ack(ack_data_t* acks, unsigned int pid, unsigned int msg_no) {
    // Acquire lock for reading
    if (pthread_rwlock_rdlock(&ack_lock)) {
        return ERROR_LOCK;
    }

    // Find ack counter bound to given pid
    ack_counter_t* this_counter = get_ack_counter(acks, pid);
    if (this_counter == NULL) {
        return ERROR_PID;
    }

    int acked = this_counter->counter >= msg_no ? 1 : 0;

    // Release lock
    if (pthread_rwlock_unlock(&ack_lock)) {
        return ERROR_LOCK;
    }

    // Return result.
    return acked;
}


int free_acks(ack_data_t* acks) {
    if (acks == NULL) {
        return ERROR_MEMORY;
    }

    int error = 0;
    for (size_t i = 0; i < acks->size; ++i) {
        ack_counter_t* counter = acks->acks + i;
        if (counter != NULL) {
            free(acks->acks + i);
        } else {
            error = ERROR_MEMORY;
        }
    }
    return error;
}
