#include "utils.h"
#include <gc/gc.h>
#include <stdarg.h>
#include <string.h>

bytes_t *flat(const bytes_t *first, ...) {
  va_list args;

  va_start(args, first);

  size_t total_len = 0;
  const bytes_t *tmp = first;

  while (tmp != NULL) {
    total_len += b_len(tmp);
    tmp = va_arg(args, const bytes_t *);
  }

  va_end(args);

  bytes_t *buffer = b_new(total_len);
  if (buffer == NULL)
    return NULL;

  va_start(args, first);

  uint8_t *dest = b_d(buffer);

  tmp = first;

  while (tmp != NULL) {
    memcpy(dest, b_d(tmp), b_len(tmp));
    dest += b_len(tmp);
    tmp = va_arg(args, bytes_t *);
  }

  va_end(args);

  return buffer;
}

bytes_t *b_mul(const bytes_t *bytes, size_t n) {
  bytes_t *buf = b_new(b_len(bytes) * n);
  if (buf == NULL)
    return NULL;

  for (size_t i = 0; i < b_len(buf); i++) {
    b_at(buf, i) = b_at(bytes, i % b_len(bytes));
  }

  return buf;
}

state_t save_state(void) {
  state_t state = {0};
  __asm__ __volatile__(".intel_syntax noprefix;"
                       "mov %0, cs;"
                       "mov %1, ss;"
                       "mov %2, rsp;"
                       "pushf;"
                       "pop %3;"
                       ".att_syntax;"
                       : "=r"(state.cs), "=r"(state.ss), "=r"(state.sp),
                         "=r"(state.flags));
  return state;
}

bytes_t *b_xor(const bytes_t *first, ...) {
  va_list args;
  va_start(args, first);

  const bytes_t *tmp = first;
  size_t buf_len = b_len(first);

  while (tmp != NULL) {
    if (b_len(tmp) > buf_len)
      buf_len = b_len(tmp);
    tmp = va_arg(args, const bytes_t *);
  }

  va_end(args);

  bytes_t *buffer = b_new(buf_len);
  if (buffer == NULL)
    return NULL;

  memset(b_d(buffer), 0, buf_len);

  va_start(args, first);

  tmp = first;

  while (tmp != NULL) {
    for (size_t i = 0; i < buf_len; i++)
      b_at(buffer, i) ^= b_at(tmp, i % b_len(tmp));

    tmp = va_arg(args, bytes_t *);
  }

  va_end(args);

  return buffer;
}

uint64_t posmod(int64_t i, int64_t n) { return (i % n + n) % n; }

bytes_t *iretq_frame(state_t state, uint64_t rip) {
  bytes_t *frame = flat(p64(rip), p64(state.cs), p64(state.flags),
                        p64(state.sp), p64(state.ss), NULL);

  return frame;
}
