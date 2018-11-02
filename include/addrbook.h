#pragma once

#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* ----- DEFINITION ----- */
typedef struct {
    // Number of processes, including current one.
    size_t num_proc;
    // List of ip addresses
    struct sockaddr_in* addresses;
} addr_book_t;

/* ----- INTERFACE ----- */

/**
 * @brief Allocate new address book.
 * 
 * @param num_entries Number of entries to store in the address book.
 * @return addr_book_t* A new address book in case of sucess, NULL in case of failure.
 */
addr_book_t* alloc_addr_book(size_t num_entries);

/**
 * @brief Free resources for an address book.
 * 
 * @param addr_book The address book to free.
 */
void free_addr_book(addr_book_t* addr_book);

/**
 * @brief Add new entry to the address book.
 * 
 * @param addr_book The address book.
 * @param pid The pid of the process.
 * @param addr The address to bind to the given process.
 * @return int 0 in case of sucess, non-0 otherwise.
 */
int add_entry(addr_book_t* addr_book, size_t pid, struct sockaddr_in* addr);

/**
 * @brief Get address for given pid.
 * 
 * @param addr_book The address book.
 * @param pid The given pid.
 * @param addr Pointer to a structure in which the function stores the corresponding IP address.
 * @return int 0 in case of success, non-0 otherwise.
 */
int get_addr(addr_book_t* addr_book, size_t pid, struct sockaddr_in* addr);