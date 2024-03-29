#pragma once

/**
 * @brief Enum containing all the error codes we use.
 * 
 */
typedef enum {

  ERROR_MEMORY = -128,
  ERROR_QUEUE,
  ERROR_ADDR_NOT_FOUND,
  ERROR_SOCKET,
  ERROR_SEND,
  ERROR_CLOSE,
  ERROR_ACK_LOCK,
  ERROR_LOCK,
  ERROR_PID,
  ERROR_BIND,
  ERROR_THREAD,
  ERROR_NETWORK,
  ERROR_IO,
  ERROR_CONDITION,
  ERROR_MUTEX,
  ERROR_FILE,
  ERROR_ARGS,
  ERROR_TABLE,
  ERROR_POINTER
} error_codes;


