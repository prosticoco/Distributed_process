#include <stdlib.h>

#include "addrbook.h"
#include "error.h"

/**
 * @brief Allocate new address book.
 * 
 * @param num_entries Number of entries to store in the address book, greater than 0.
 * @return addr_book_t* A new address book in case of sucess, NULL in case of failure.
 */
addr_book_t* alloc_addr_book(size_t num_entries) {
    if (num_entries == 0) {
        return NULL;
    }

    addr_book_t* new_addr_book = malloc(sizeof(addr_book_t));
    if (!new_addr_book) {
        return NULL;
    }

    struct sockaddr_in* addresses = calloc(num_entries, sizeof(struct sockaddr_in));
    if (!addresses) {
        free(new_addr_book);
        return NULL;
    }
    new_addr_book->addresses = addresses;
    return new_addr_book;
}

/**
 * @brief Free resources for an address book.
 * 
 * @param addr_book The address book to free. If it is NULL, no operation will be performed.
 */
void free_addr_book(addr_book_t* addr_book) {
    if (addr_book != NULL) {
        free(addr_book->addresses);
        free(addr_book);
    }
}

/**
 * @brief Add new entry to the address book.
 * 
 * @param addr_book The address book.
 * @param pid The pid of the process.
 * @param addr The address to bind to the given process.
 * @return int 0 in case of sucess, non-0 otherwise.
 */
int add_entry(addr_book_t* addr_book, size_t pid, struct sockaddr_in addr) {
    if (!addr_book || !addr_book->addresses) {
        return ERROR_MEMORY;
    }
    if (pid > addr_book->num_proc + 1) {
        return ERROR_PID;
    }
    addr_book->addresses[pid-1] = addr;
    return 0;
}

/**
 * @brief Get address for given pid.
 * 
 * @param addr_book The address book.
 * @param pid The given pid.
 * @param addr Pointer to a structure in which the function stores the corresponding IP address.
 * @return int 0 in case of success, non-0 otherwise.
 */
int get_addr(addr_book_t* addr_book, size_t pid, struct sockaddr_in* addr) {
    if (!addr_book || !addr_book->addresses || !addr) {
        return ERROR_MEMORY;
    }
    if (pid > addr_book->num_proc + 1) {
        return ERROR_PID;
    }
    *addr = addr_book->addresses[pid-1];
    return 0;
}