#include "addrbook.h"
#include "structure.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>


// creates an address book for all nodes listed in membership file
int init_addrbook(addr_book_t* book, size_t num_proc, da_process_t* proc_list) {
    // set the book size to the number of nodes
    book->size = num_proc;
    // allocate memory for the list of addresses
    book->listaddr = calloc(num_proc, sizeof(addr_entry_t));
    if (book->listaddr == NULL) {
        return ERROR_MEMORY;
    }
    // set all the list of entries to zero
    memset(book->listaddr, 0, num_proc*sizeof(addr_entry_t));
    struct sockaddr_in * curr_addr;
    // go over all the list of entries and update the addresses of the book
    for (int i = 0 ; i < num_proc; i++) {
        // get the address of the address field :)
        curr_addr = &(book->listaddr[i].address);
        book->listaddr[i].process_id = proc_list[i].uid;
        curr_addr->sin_family = AF_INET;
        curr_addr->sin_port = proc_list[i].port_num;
        curr_addr->sin_addr = proc_list[i].ipv4_addr;
    }
    return 0;
}

// updates address passed in arguments according to the proc_id
// returns 0 upon success, -1 if wrong proc_id
int find_addrbook(addr_book_t* book, unsigned int proc_id, struct sockaddr_in* address){
  addr_entry_t curr_entry;
  for(int i = 0; i < book->size; i++){
      curr_entry = book->listaddr[i];
      if(curr_entry.process_id == proc_id){
        *address = curr_entry.address;
        return 0;
      }
  }
  return ERROR_ADDR_NOT_FOUND;
}

int free_addrbook(addr_book_t* book){
  free(book->listaddr);
  return 0;
}
