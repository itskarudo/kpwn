#include "io.h"
#include "utils.h"
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

io_t io_new(const char *path, int flags) {
  io_t self;
  self._fd = open(path, flags);
  return self;
}

void io_close(io_t self) { close(self._fd); }

bytes_t io_read(io_t self, size_t n) {

  bytes_t buffer = b_new(n);
  if (buffer._data == NULL)
    return buffer;

  buffer._len = read(self._fd, b_d(buffer), n);

  return buffer;
}

bytes_t io_readuntil(io_t self, bytes_t delim) {

  bytes_t buffer = io_read(self, b_len(delim));

  while (b_cmp(b_slice(buffer, -b_len(delim), -1), delim) != 0) {
    bytes_t tmp = io_read(self, 1);
    b_append(&buffer, tmp);
  }

  return buffer;
}

bytes_t io_readline(io_t self) { return io_readuntil(self, b("\n")); }

void io_write(io_t self, bytes_t v) { write(self._fd, b_d(v), b_len(v)); }

int io_ioctl(io_t self, unsigned long request, ...) {
  va_list args;
  va_start(args, request);
  return ioctl(self._fd, request, va_arg(args, void *));
}

void __destroy_io_t(io_t *selfp) { io_close(*selfp); }
void io_destroy(io_t self) { __destroy_io_t(&self); }
