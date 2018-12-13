#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dependencies.h"
#include "error.h"

/**
 * @brief Test if a process depends on another.
 * 
 * @param dependencies The dependency object to access.
 * @param from The source of the dependency.
 * @param to The target of the dependency.
 * @return int 0 if no dependence, > 0 in case of dependence, < 0 in case of error.
 */
static int depends_on(dependencies_t* dependencies, size_t from, size_t to) {
    if (NULL == dependencies || NULL == dependencies->dep_lists || from >= dependencies->num_proc) {
        return ERROR_ARGS;
    }

    dependency_list_t* from_list = get_dependencies(dependencies, from);
    // BUG: from_list->pid_list == NULL
    if (NULL == from_list) {
        return ERROR_MEMORY;
    }
    // This means the process from has no dependency, so we return 0.
    if (NULL == from_list->pid_list) {
        return 0;
    }

    for (size_t i = 0; i < from_list->list_len; ++i) {
        if (from_list->pid_list[i] == to) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Allocate new dependency matrix, with given number of processes.
 * 
 * @param num_proc Number of processes.
 * @return dependencies_t* Pointer to allocated dependency matrix, with empty dependency lists, NULL in case of failure.
 */
dependencies_t* alloc_dependencies(size_t num_proc) {
    // Allocate required structures.
    dependencies_t* dependencies = calloc(1, sizeof(dependencies_t));
    dependency_list_t* list = calloc(num_proc, sizeof(dependency_list_t));
    if (NULL == dependencies || NULL == list || 0 == num_proc) {
        free(dependencies);
        free(list);
        return NULL;
    }

    // Set all fields to default values.
    dependencies->dep_lists = list;
    dependencies->num_proc = num_proc;
    for (size_t i = 0; i < num_proc; ++i) {
        dependencies->dep_lists[i].pid_list = NULL;
        dependencies->dep_lists[i].list_len = 0;
    }

    return dependencies;
}

/**
 * @brief Allocate new dependency matrix, which is the reverse of the one given.
 * 
 * @param reference The dependency matrix to reverse.
 * @return dependencies_t* New dependency matrix, reverse of the given one.
 */
dependencies_t* alloc_reverse_dependencies(dependencies_t* reference) {
    if (NULL == reference) {
        return NULL;
    }
    const size_t num_proc = reference->num_proc;
    // Allocate required structures.
    dependencies_t* dependencies = calloc(1, sizeof(dependencies_t));
    dependency_list_t* lists = calloc(num_proc, sizeof(dependency_list_t));
    if (NULL == dependencies || NULL == lists || 0 == num_proc) {
        free(dependencies);
        free(lists);
        return NULL;
    }

    // Set all fields to default values.
    dependencies->dep_lists = lists;
    dependencies->num_proc = num_proc;

    // Set reverse dependency lists.
    for (size_t from = 0; from < num_proc; ++from) {
        // Find all pids that depend on process l.
        size_t tmp_pids[num_proc];
        size_t dep_cntr = 0;
        for (size_t to = 0; to < num_proc; ++to) {
            // Get dependency list of process i.
            // Add process i to the temporary buffer if it depends on l.
            int dep_test = depends_on(reference, to, from);
            if (0 > dep_test) {
                free_dependencies(dependencies);
                return NULL;
            }
            if (0 < dep_test) {
                tmp_pids[dep_cntr] = to;
                dep_cntr++;
            }
        }
        // Copy the pids we found to the dependency object we are building.
        dependency_list_t* from_list = get_dependencies(dependencies, from);
        if (NULL == from_list) {
            free_dependencies(dependencies);
            return NULL;
        }
        from_list->pid_list = calloc(dep_cntr, sizeof(size_t));
        if (NULL == from_list->pid_list || NULL == memcpy(from_list->pid_list, tmp_pids, dep_cntr * sizeof(size_t))) {
            free_dependencies(dependencies);
            return NULL;
        }
        from_list->list_len = dep_cntr;
    }

    return dependencies;
}

/**
 * @brief Free resources of a dependency matrix.
 * 
 * @param dependencies A valid dependency object to free.
 */
void free_dependencies(dependencies_t* dependencies) {
    // Nothing to do if dependencies is NULL.
    if (NULL == dependencies) {
        fprintf(stderr, "Warning: dependencies: attempting to free a NULL dependency object.");
        return;
    }
    // Free all dependency lists of the object.
    size_t num_lists = dependencies->num_proc;
    dependencies->num_proc = 0;
    if (NULL == dependencies->dep_lists) {
        return;
    }
    for (size_t l = 0; l < num_lists; ++l) {
        dependency_list_t* list = &(dependencies->dep_lists[l]);
        free(list->pid_list);
        list->pid_list = NULL;
    }
    // Free the list of dependency lists.
    free(dependencies->dep_lists);
}

/**
 * @brief Set the dependencies for a given pid to a given list of pid's.
 * 
 * @param dependencies The dependencies_t object to modify.
 * @param from The pid for which to set the dependency list. Starts at 0.
 * @param pid_list The dependency list. Its values are *copied* to a newly allocated list.
 * @param pid_list_len The length of the dependency list.
 * @return int 0 in case of success, non-zero otherwise.
 */
int set_dependencies(dependencies_t* dependencies, size_t from, size_t* pid_list, size_t pid_list_len) {
    // Check arguments.
    if (NULL == dependencies || NULL == pid_list) {
        return ERROR_MEMORY;
    }
    // Get dependency list to set.
    // Free it and copy the new value to it.
    dependency_list_t* list = get_dependencies(dependencies, from);
    if (NULL == list) {
        return ERROR_MEMORY;
    }

    if (pid_list_len > 0) {
        list->pid_list = realloc(list->pid_list, pid_list_len * sizeof(size_t));
        if (NULL == list->pid_list) {
            return ERROR_MEMORY;
        }
        if (NULL == memcpy(list->pid_list, pid_list, pid_list_len * sizeof(size_t))) {
            return ERROR_MEMORY;
        }
    }
    list->list_len = pid_list_len;
    return 0;
}

/**
 * @brief Get the list of dependencies for a given pid.
 * 
 * @param dependencies The dependencies_t object to access.
 * @param from The pid for which to get the dependency list.
 * @return size_t* List of dependencies, or NULL in case of failure.
 *                 /!\ Points *directly* to the inner local list of the
 *                 dependency object, so may change between calls if the
 *                 object is modified with set_dependencies().
 */
dependency_list_t* get_dependencies(dependencies_t* dependencies, size_t from) {
    if (NULL == dependencies || NULL == dependencies->dep_lists || from >= dependencies->num_proc) {
        return NULL;
    }
    return &(dependencies->dep_lists[from]);
}