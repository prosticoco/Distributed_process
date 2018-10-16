#ifndef ADDRBOOK_H
#define ADDRBOOK_H

#include "structure.h"
#include "error.h"


// creates an address book for all nodes listed in membership file
int init_addrbook(addr_book_t* book, size_t num_proc, da_process_t* proc_list);

// updates address passed in arguments according to the proc_id
// returns 0 upon success, -1 if wrong proc_id
int find_addrbook(addr_book_t* book, unsigned int proc_id, struct sockaddr_in* address);

int free_addrbook(addr_book_t* book);

#endif
