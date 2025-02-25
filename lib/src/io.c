#include "io.h"
#include "utils.h"
#include <fcntl.h>
#include <gc/gc.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

void io_finalizer(io_t *self, void *user_data) {
  (void)user_data;

  close(self->_fd);

  free(self);
}

io_t *io_new(const char *path, int flags) {
  io_t *self = GC_MALLOC_ATOMIC(sizeof(io_t));
  if (self == NULL)
    return NULL;

  self->_fd = open(path, flags);
  if (self->_fd == -1)
    return NULL;

  GC_register_finalizer(self, (GC_finalization_proc)io_finalizer, NULL, NULL,
                        NULL);

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

  while (b_cmp(b_slice(buffer, -b_len(delim), -1), delim) != 0) {
    bytes_t *tmp = io_read(self, 1);
    buffer = flat(buffer, tmp, NULL);
  }

  return buffer;
}

bytes_t *io_readline(io_t *self) { return io_readuntil(self, b("\n")); }

void io_write(io_t *self, bytes_t *v) { write(self->_fd, v->_data, v->_len); }

int io_ioctl(io_t *self, unsigned long request, ...) {
  va_list args;
  va_start(args, request);
  return ioctl(self->_fd, request, va_arg(args, void *));
}
