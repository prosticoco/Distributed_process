#include "data.h"
#include "error.h"
#include "dependencies.h"
#include "layers.h"

/*
Pending.c contains all the code to implement the lcb layer, ie all the structures needed for lcb : vector clocks, pending table

The vector clock is just a dynamically allocated list of unsigned ints

The Pending table is a table of doubly linked list (one linked list per process ID), every node of the list contains a pointer to a vector clock
*/

/**
 * @brief Function which allocates memory for a new vector clock
 * 
 * @param clocky the new vector clock we want to initialize
 * @param num_proc the total number of nodes (processes) (ie the number of fields per vector clock)
 * @param values a pointer to a list of values to fill the newly created vector
 * @return int 0 upon success, negative code otherwise
 */
int alloc_vector(vec_clock_t* clocky,unsigned int num_proc,unsigned int* values);

/**
 * @brief frees any given vector clock
 * 
 * @param clocky the vector clock to free
 * @return int  0 upon success, negative code if invalid pointer
 */
int destroy_vector(vec_clock_t* clocky);

/**
 * @brief initializes a linked list of vectors
 * 
 * @param list the list to initialize
 * @param num_proc the number of processes
 * @return int always 0
 */
int init_veclist(vec_list_t* list,size_t num_proc);

/**
 * @brief adds a new vector to one of the linked lists
 * 
 * @param clock the vector to add
 * @param list the linked list in which we add the vector
 * @return 0 on success, negative error code otherwise
 */
int add_veclist(vec_clock_t clock,vec_list_t* list);

/**
 * @brief removes a vector from the linked list
 * 
 * @param node the node from the list we want to remove 
 * @param list the list from which we remove the node
 * @return 0 upon success, negative code otherwise
 */
int remove_veclist(vec_node_t* node,vec_list_t* list);

/**
 * @brief frees completlely a linked list (also takes care of freeing the vector clocks)
 * 
 * @param list the linked list to free
 * @return 0 upon success, negative error code otherwise
 */
int free_veclist(vec_list_t* list);

/**
 * @brief initializes the list of linked lists
 * 
 * @param pending the pending structure to initialize
 * @param num_proc the number of processes
 * @return 0 upon success, negative error code otherwise
 */
int init_pending_lcb(lcb_pending_t* pending,size_t num_proc);

/**
 * @brief frees completely the pending structure
 * 
 * @param pending the pending structure to free
 * @return 0 upon success, negative error code otherwise
 */
int destroy_pending_lcb(lcb_pending_t* pending);

/**
 * @brief adds a vector to the pending structures, chooses the linked list according to the pid
 * 
 * @param pending the pending structure
 * @param pid the pid of the process
 * @param clock the vector clock
 * @return 0 upon success, negative code otherwise
 */
int add_pending_lcb(lcb_pending_t* pending,unsigned int pid,vec_clock_t clock);

/**
 * @brief initializes the current's process vector clock, which has to be locked with a mutex
 * 
 * @param clocky current processes pointer to its vector clock
 * @param num_proc the size of the vector clock
 * @return 0 upon success, negative error code otherwise
 */
int init_vector_mutex(vec_clock_t* clocky,unsigned int num_proc);

/**
 * @brief updates the current vector clock of the process for a given pid 
 * 
 * @param data the data of the process
 * @param pid the pid of the process
 * @param seq an reference to an unsigned int so that we can use the new value of the clock
 * @return 0 upon success, negative error code otherwise
 */
int update_vec_clock(net_data_t* data, unsigned int pid,unsigned int * seq);

/**
 * @brief allocates a copy of the current vector clock of the process
 * used to create a new message which copies the values of the vector clock
 * @param data the data of the process
 * @param clocky the message's vector clock
 * @return 0 upon success, negative error code otherwise
 */
int get_vec_clock_copy(net_data_t* data, vec_clock_t* clocky);

/**
 * @brief tests wether a received vector clock is valid, ie the message can be delivered
 * by using the dependencies list ie compares W >= V for two vectors
 * according to dependencies
 * 
 * @param data the data of the current process
 * @param vector the vector clock received
 * @param pid the pid of the original sender of the message
 * @return 1 if valid (message can be delivered) 0 if not, < 0 if error
 */
int test_vec_clock(net_data_t* data,vec_clock_t* vector,unsigned int pid);

/**
 * @brief Searches through all the pending structure to find vectors for which 
 * the corresponding message can be delivered
 * Whenever it finds a valid vector, it deletes it from the list and delivers
 * the corresponding message
 * @param data the current process data
 * @return 1 if at least one vector was valid, 0 if not, < 0 if error
 */
int find_pending(net_data_t* data);