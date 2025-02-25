#include "bytes.h"
#include "utils.h"
#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bytes_t *b_new(size_t n) {
  bytes_t *self = GC_MALLOC_ATOMIC(sizeof(bytes_t) + n);

  if (self == NULL)
    return NULL;

  self->_len = n;
  return self;
}

bytes_t *b_new_v(const uint8_t *bytes, size_t n) {

  bytes_t *self = b_new(n);

  memcpy(self->_data, bytes, n);
  return self;
}

bytes_t *b_slice(const bytes_t *original, size_t start, size_t end) {

  const uint8_t *start_ptr = b_d(original) + posmod(start, b_len(original));

  size_t len =
      posmod(end, b_len(original)) - posmod(start, b_len(original)) + 1;

  bytes_t *self = b_new_v(start_ptr, len);
  return self;
}

int b_cmp(const bytes_t *b1, const bytes_t *b2) {
  if (b_len(b1) != b_len(b2))
    return 1;
  return memcmp(b_d(b1), b_d(b2), b_len(b1));
}

const char *b_s(const bytes_t *self) {
  char *s = GC_MALLOC_ATOMIC(b_len(self) * 4 + 4);

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
