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
 * @brief Free resources of a dependency matrix.
 * 
 * @param dependencies The dependency matrix to free.
 */
void free_dependencies(dependencies_t* dependencies);

/**
 * @brief Add dependency of one process to another.
 * 
 * @param dependencies The dependency matrix to operate on.
 * @param from The process who depends on another process.
 * @param to The process which is a dependency of the `from` process.
 * @return int 0 in case of success, non-zero otherwise.
 */
int set_dependency(dependencies_t* dependencies, size_t from, size_t to);

/**
 * @brief Check dependency of one process to another.
 * 
 * @param dependencies The dependency matrix to operate on.
 * @param from The process who may depend on another process.
 * @param to The process which may be a dependency of the `from` process.
 * @return int 1 if `from` depends on `to`, 0 if not.
 */
int get_dependency(dependencies_t* dependencies, size_t from, size_t to);