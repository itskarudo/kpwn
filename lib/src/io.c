#include "io.h"
#include "utils.h"
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

io_t *io_new(const char *path, int flags) {
  io_t *self = malloc(sizeof(io_t));
  if (self == NULL)
    return NULL;

  self->_fd = open(path, flags);
  if (self->_fd < 0) {
    io_free(self);
    return NULL;
  }

  return self;
}

void io_close(io_t *self) { close(self->_fd); }

bytes_t *io_read(io_t *self, size_t n) {

  bytes_t *buffer = b_new(n);
  if (buffer == NULL)
    return NULL;

  buffer->_len = read(self->_fd, buffer->_data, n);

  return buffer;
}

bytes_t *io_readuntil(io_t *self, bytes_t *delim) {

  bytes_t *buffer = io_read(self, b_len(delim));

  bytes_t *end = b_slice(buffer, -b_len(delim), -1);

  while (b_cmp(end, delim) != 0) {
    bytes_t *tmp = io_read(self, 1);
    buffer = flat(buffer, tmp, NULL);
    b_free(end);
    end = b_slice(buffer, -b_len(delim), -1);
  }

  b_free(end);

  return buffer;
}

bytes_t *io_readline(io_t *self) { return io_readuntil(self, b("\n")); }

void io_write(io_t *self, bytes_t *v) { write(self->_fd, v->_data, v->_len); }

int io_ioctl(io_t *self, unsigned long request, ...) {
  va_list args;
  va_start(args, request);
  return ioctl(self->_fd, request, va_arg(args, void *));
}

void __free_io_t(io_t **selfp) {
  io_close(*selfp);
  free(*selfp);
}
void io_free(io_t *self) { __free_io_t(&self); }
