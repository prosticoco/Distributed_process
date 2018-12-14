
#include <stdlib.h>
#include <string.h>

#include "pending.h"




int alloc_vector(vec_clock_t* clocky,unsigned int num_proc,unsigned int* values){
    clocky->num_proc = num_proc;
    clocky->vector = calloc(num_proc,sizeof(unsigned int));
    if(clocky->vector == NULL){
        return ERROR_MEMORY;
    }
    memcpy(clocky->vector,values,num_proc*sizeof(unsigned int));
    return 0;
}

int destroy_vector(vec_clock_t* clocky){
    if(clocky == NULL || clocky->vector == NULL){
        return ERROR_POINTER;
    }
    free(clocky->vector);
    return 0;
}



int init_veclist(vec_list_t* list,size_t num_proc){
    list->num_elems = 0;
    list->head = NULL;
    list->tail = NULL;
    return 0;
}

int add_veclist(vec_clock_t clock,vec_list_t* list){
    int error;
    if(clock.vector == NULL){
        return ERROR_POINTER;
    }
    vec_node_t* node = malloc(sizeof(vec_node_t));
    if(node == NULL){
        return ERROR_MEMORY;
    }
    error = alloc_vector(&(node->clock),clock.num_proc,clock.vector);
    if(error){
        return error;
    }
    node->next = NULL;
    if(list->num_elems == 0){
        list->head = node;
        list->tail = node;
        node->previous = NULL;
    }else{     
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    }  
    list->num_elems += 1;
    return 0;
}

int remove_veclist(vec_node_t* node,vec_list_t* list){
    int error;
    if(node == NULL || list == NULL || node->clock.vector == NULL){
        return ERROR_POINTER;
    }
    if(list->head == node){
        list->head = node->next;
    }else{
        node->previous->next = node->next;
    }
    if(list->tail == node){
        list->tail = node->previous;
    }else{
        node->next->previous = node->previous;
    }
    list->num_elems -= 1;
    error = destroy_vector(&(node->clock));
    if(error){
        return error;
    }
    free(node);
    return 0;
}

int free_veclist(vec_list_t* list){
    int error;
    if(list == NULL){
        return ERROR_POINTER;
    }
    vec_node_t* ptr = list->head;
    while(ptr != NULL){
        error = remove_veclist(ptr,list);
        if(error){
            return error;
        }
        ptr = list->head;
    }
    list->tail = NULL;
    list->head = NULL;
    return 0;
}

int init_pending_lcb(lcb_pending_t* pending,size_t num_proc){
    if(pending == NULL){
        return ERROR_POINTER;
    }
    pending->lists = calloc(num_proc,sizeof(vec_list_t));
    if(pending->lists == NULL){
        return ERROR_MEMORY;
    }
    pending->num_proc = num_proc;
    return 0;
}

int destroy_pending_lcb(lcb_pending_t* pending){
    if(pending == NULL || pending->lists == NULL){
       return ERROR_MEMORY;
    }
    free(pending->lists);
    pending->num_proc = 0;
    return 0;
}

int add_pending_lcb(lcb_pending_t* pending,unsigned int pid,vec_clock_t clock){
    int error;
    if(pending == NULL || pending->lists == NULL){
        return ERROR_POINTER;
    }
    if(pid > pending->num_proc){
        return ERROR_PID;
    }
    error = add_veclist(clock,&(pending->lists[pid -1]));
    if(error){
        return error;
    }
    return 0;
}


int init_vector_mutex(vec_clock_t* clocky,unsigned int num_proc){
    unsigned int init[num_proc];
    memset(init,0,sizeof(unsigned int)*num_proc);
    int error = pthread_mutex_init(&(clocky->mutex),NULL);
    if(error){
        printf("Error initializing mutex \n");
        return ERROR_MUTEX;
    }
    error = alloc_vector(clocky,num_proc,init);
    if(error){
        printf("Error allocating vector clock \n");
        return error;
    }
    return 0;
}

int update_vec_clock(net_data_t* data, unsigned int pid){
    if(pid > data->num_proc){
        return ERROR_PID;
    }
    pthread_mutex_lock(&(data->vector_clock->mutex));
    data->vector_clock->vector[pid-1] += 1;
    pthread_mutex_unlock(&(data->vector_clock->mutex));
    return 0;
}

int get_vec_clock_copy(net_data_t* data, vec_clock_t* clocky){
    int error;
    pthread_mutex_lock(&(data->vector_clock->mutex));
    error = alloc_vector(clocky,data->num_proc,data->vector_clock->vector);
    if(error){
        printf("Error allocating vector in get_vec_clock_copy \n");
        return error;
    }
    pthread_mutex_unlock(&(data->vector_clock->mutex));
    return 0;
}



int test_vec_clock(net_data_t* data,vec_clock_t* vector,unsigned int pid){
    dependency_list_t* dep = get_dependencies(data->dependencies,pid);
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
    int error;
    lcb_pending_t* pending = data->lcb_pending;
    unsigned int num_proc = pending->num_proc;
    int found_one = 0;
    for(int i = 0 ; i < num_proc; i++){
        vec_node_t* ptr = pending->lists[i].head;
        while(ptr != NULL){
            if(test_vec_clock(data,&(ptr->clock),i+1)){
                found_one = 1;
                error = update_vec_clock(data,i+1);
                if(error){
                    printf("Error updating vector clock\n");
                    return error;
                }
                ptr = ptr->next;
                error = remove_veclist(ptr,&(pending->lists[i]));
                if(error){
                    printf("Error removing vector from pending \n");
                    return error;
                }
            }
        }
    }
    return found_one;
}




