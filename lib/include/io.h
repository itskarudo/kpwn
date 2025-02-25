#pragma once

#include "bytes.h"
#include <fcntl.h>

typedef struct {
  int _fd;
} io_t;

#ifdef __cplusplus
extern "C" {
#endif

io_t *io_new(const char *path, int flags);
void io_close(io_t *);

bytes_t *io_read(io_t *, size_t n);
bytes_t *io_readuntil(io_t *, bytes_t *delim);
bytes_t *io_readline(io_t *);

void io_write(io_t *, bytes_t *);

int io_ioctl(io_t *, unsigned long request, ...);

#ifdef __cplusplus
}
#endif
