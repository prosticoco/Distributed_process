#include "data.h"
#include "error.h"


int alloc_vector(vec_clock_t* clocky,unsigned int num_proc,unsigned int* values);

int destroy_vector(vec_clock_t* clocky);

int init_veclist(vec_list_t* list,size_t num_proc);

int add_veclist(vec_clock_t clock,vec_list_t* list);

int remove_veclist(vec_node_t* node,vec_list_t* list);

int free_veclist(vec_list_t* list);

int init_pending_lcb(lcb_pending_t* pending,size_t num_proc);

int destroy_pending_lcb(lcb_pending_t* pending);

int add_pending_lcb(lcb_pending_t* pending,unsigned int pid,vec_clock_t clock);