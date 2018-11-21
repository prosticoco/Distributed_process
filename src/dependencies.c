#include <stdlib.h>
#include <stdint.h>

#include "dependencies.h"
#include "error.h"

static inline int matrix_index(size_t* index, size_t from, size_t to, size_t num_proc) {
    if (from < 1 || to < 1 || from > num_proc || to > num_proc) {
        return 1;
    }
    *index = (from - 1) * num_proc + to - 1;
    return 0;
}

/**
 * @brief Allocate new dependency matrix, with given number of processes.
 * 
 * @param num_proc Number of processes.
 * @return dependencies_t* Pointer to allocated dependency matrix, NULL in case of failure.
 */
dependencies_t* alloc_dependencies(size_t num_proc) {
    if (0 == num_proc) {
        return NULL;
    }

    dependencies_t* dependencies = malloc(sizeof(dependencies_t));
    if (NULL == dependencies) {
        return NULL;
    }

    const size_t matrix_linsize = num_proc*num_proc;
    int* lin_matrix = calloc(matrix_linsize, sizeof(int*));
    if (NULL == lin_matrix) {
        free(dependencies);
        return NULL;
    }

    dependencies->lin_matrix = lin_matrix;
    dependencies->num_proc = num_proc;
    return dependencies;
}

/**
 * @brief Free resources of a dependency matrix.
 * 
 * @param dependencies The dependency matrix to free.
 */
void free_dependencies(dependencies_t* dependencies) {
    if (NULL != dependencies) {
        free(dependencies->lin_matrix);
        free(dependencies);
    }
}

/**
 * @brief Add dependency of one process to another.
 * 
 * @param dependencies The dependency matrix to operate on.
 * @param from The process who depends on another process.
 * @param to The process which is a dependency of the `from` process.
 * @return int 0 in case of success, non-zero otherwise.
 */
int set_dependency(dependencies_t* dependencies, size_t from, size_t to) {
    if (NULL == dependencies) {
        return ERROR_MEMORY;
    }
    size_t index;
    if (0 != matrix_index(&index, from, to, dependencies->num_proc)) {
        return ERROR_ARGS;
    }
    dependencies->lin_matrix[index] = 1;
    return 0;
}

/**
 * @brief Check dependency of one process to another.
 * 
 * @param dependencies The dependency matrix to operate on.
 * @param from The process who may depend on another process.
 * @param to The process which may be a dependency of the `from` process.
 * @return int 1 if `from` depends on `to`, 0 if not, <0 in case of failure.
 */
int get_dependency(dependencies_t* dependencies, size_t from, size_t to) {
    if (NULL == dependencies) {
        return ERROR_MEMORY;
    }
    size_t index;
    if (0 != matrix_index(&index, from, to, dependencies->num_proc)) {
        return ERROR_ARGS;
    }
    return dependencies->lin_matrix[index];
}