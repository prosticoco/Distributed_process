#include <stdio.h>
#include <stdlib.h>

#include "dependencies.h"

#define NUM_PROC 5

int main(void) {
    dependencies_t* test = alloc_dependencies(NUM_PROC);
    if (NULL == test) {
        printf("Allocation failed.\n");
    }

    size_t dep_buffer[NUM_PROC];
    for (size_t from = 0; from < NUM_PROC; ++from) {
        size_t dep_len = ((size_t) rand() % 5) + 1;
        for (size_t to = 1; )
    }

    free_dependencies(test);
    printf("Tests terminated successfully!\n");
    return 0;
}