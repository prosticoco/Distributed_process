#pragma once

#include "data.h"

/**
 * @brief Allocate new dependency matrix, with given number of processes.
 * 
 * @param num_proc Number of processes.
 * @return dependencies_t* Pointer to allocated dependency matrix, NULL in case of failure.
 */
dependencies_t* alloc_dependencies(size_t num_proc);

/**
 * @brief Allocate new dependency matrix, which is the reverse of the one given.
 * 
 * @param reference The dependency matrix to reverse.
 * @return dependencies_t* New dependency matrix, reverse of the given one.
 */
dependencies_t* alloc_reverse_dependencies(dependencies_t* reference);

/**
 * @brief Free resources of a dependency matrix.
 * 
 * @param dependencies The dependency matrix to free.
 */
void free_dependencies(dependencies_t* dependencies);

/**
 * @brief Set the dependencies for a given pid to a given list of pid's.
 * 
 * @param dependencies The dependencies_t object to modify.
 * @param from The pid for which to set the dependency list.
 * @param pid_list The dependency list.
 * @param pid_list_len The length of the dependency list.
 * @return int 0 in case of success, non-zero otherwise.
 */
int set_dependencies(dependencies_t* dependencies, size_t from, size_t* pid_list, size_t pid_list_len);

/**
 * @brief Get the list of dependencies for a given pid.
 * 
 * @param dependencies The dependencies_t object to access.
 * @param from The pid for which to get the dependency list.
 * @return size_t* List of dependencies.
 */
dependency_list_t* get_dependencies(dependencies_t* dependencies, size_t from);