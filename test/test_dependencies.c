#include <stdio.h>
#include <stdlib.h>

#include "dependencies.h"

#define NUM_PROC 5

void print_deps(dependencies_t* dependencies, size_t from) {
    dependency_list_t* deps = get_dependencies(dependencies, from);
    if (NULL == deps) {
        printf("Get dependencies returned NULL.\n");
        exit(1);
    }

    printf("%zu dependencies for process %zu: ", deps->list_len, from);
    for (size_t i = 0; i < deps->list_len; ++i) {
        printf("%zu ", deps->pid_list[i]);
    }
    printf("\n");
}

int main(void) {
    dependencies_t* test = alloc_dependencies(NUM_PROC);
    if (NULL == test) {
        printf("Allocation failed.\n");
        return 1;
    }

    size_t dep_buffer[NUM_PROC][NUM_PROC] = {
        {1, 2, 4}, // p0
        {},
        {0, 3}
    };
    int res = set_dependencies(test, 0, dep_buffer[0], 3);
    res += set_dependencies(test, 1, dep_buffer[1], 0);
    res += set_dependencies(test, 2, dep_buffer[2], 2);
    if (res) {
        printf("%d exit code for set_dependencies.\n", res);
        return res;
    }

    printf("\nDependencies of the object: \n");
    for (size_t pid = 0; pid < NUM_PROC; ++pid) {
        print_deps(test, pid);
    }

    dependencies_t* rev_test = alloc_reverse_dependencies(test);
    if (NULL == rev_test) {
        printf("Allocation of reverse dependencies failed.\n");
        return 1;
    }

    printf("\nDependencies of the reverse object: \n");
    for (size_t pid = 0; pid < NUM_PROC; ++pid) {
        print_deps(rev_test, pid);
    }

    dependencies_t* rev_rev_test = alloc_reverse_dependencies(rev_test);
    if (NULL == rev_rev_test) {
        printf("Allocation of reverse reverse dependencies failed.\n");
        return 1;
    }

    printf("\nDependencies of the reverse reverse object: \n");
    for (size_t pid = 0; pid < NUM_PROC; ++pid) {
        print_deps(rev_rev_test, pid);
    }

    free_dependencies(test);
    free_dependencies(rev_test);
    printf("Tests terminated successfully!\n");
    return 0;
}