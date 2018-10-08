




#ifndef ERROR_H
#define ERROR_H

typedef enum {

  ERROR_MEMORY = -128,
  ERROR_QUEUE,
  ERROR_ADDR_NOT_FOUND,
  ERROR_SOCKET,
  ERROR_SEND,
  ERROR_CLOSE
} error_codes;

#endif
