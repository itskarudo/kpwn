#pragma once

#include "bytes.h"
#include "log.h"

typedef struct {
  bytes_t cyclic_alphabet;
  size_t cyclic_size;
  enum LogLevel log_level;
} context_t;

extern context_t context;
