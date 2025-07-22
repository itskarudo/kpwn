#include "bytes.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bytes_t b_new(size_t n) {
  bytes_t res;
  res._data = malloc(n);

  if (res._data == NULL) {
    res._len = 0;
    return res;
  }

  res._len = n;
  return res;
}

bytes_t b_new_v(const uint8_t *bytes, size_t n) {

  bytes_t self = b_new(n);
  if (self._data == NULL)
    return self;

  memcpy(self._data, bytes, n);
  return self;
}

bytes_t b_slice(const bytes_t original, size_t start, size_t end) {

  uint8_t *start_ptr = b_d(original) + posmod(start, b_len(original));
  size_t len =
      posmod(end, b_len(original)) - posmod(start, b_len(original)) + 1;

  return (bytes_t){._data = start_ptr, ._len = len};
}

int b_cmp(const bytes_t b1, const bytes_t b2) {
  if (b_len(b1) != b_len(b2))
    return 1;
  return memcmp(b_d(b1), b_d(b2), b_len(b1));
}

const char *b_str(const bytes_t self) {
  char *s = malloc(b_len(self) * 4 + 4);

  char *ptr = s;
  *ptr++ = 'b';
  *ptr++ = '\'';

  for (size_t i = 0; i < b_len(self); i++) {
    if (b_at(self, i) == '\\') {
      *ptr++ = '\\';
      *ptr++ = '\\';
    } else if (b_at(self, i) == '\t') {
      *ptr++ = '\\';
      *ptr++ = 't';
    } else if (b_at(self, i) == '\n') {
      *ptr++ = '\\';
      *ptr++ = 'n';
    } else if (b_at(self, i) == '\r') {
      *ptr++ = '\\';
      *ptr++ = 'r';
    } else if (b_at(self, i) >= 0x20 && b_at(self, i) <= 0x7e) {
      *ptr++ = b_at(self, i);
    } else {
      sprintf(ptr, "\\x%02x", b_at(self, i));
      ptr += 4;
    }
  }

  *ptr++ = '\'';
  *ptr = '\0';

  return s;
}

void b_append(bytes_t *selfp, const bytes_t other) {
  bytes_t self = *selfp;
  *selfp = flat(2, *selfp, other);
  b_destroy(self);
}

void __destroy_bytes_t(bytes_t *selfp) { free(selfp->_data); }

void b_destroy(bytes_t self) { __destroy_bytes_t(&self); }
