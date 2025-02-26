#pragma once

#include <stddef.h>
#include <stdint.h>
typedef struct {
  size_t _len;
  uint8_t _data[];
} bytes_t;

#ifdef __cplusplus
extern "C" {
#endif

bytes_t *b_new(size_t n);
bytes_t *b_new_v(const uint8_t *, size_t n);
bytes_t *b_slice(const bytes_t *, size_t start, size_t end);
int b_cmp(const bytes_t *, const bytes_t *);
const char *b_s(const bytes_t *);
void b_append(bytes_t **, const bytes_t *other);

#ifdef __cplusplus
}
#endif

#define b(v) (b_new_v((uint8_t *)(v), sizeof(v) - 1))
#define b_d(v) ((v)->_data)
#define b_at(v, i) ((v)->_data[i])
#define b_len(v) ((v)->_len)

#define p8(v)                                                                  \
  __extension__({                                                              \
    uint8_t _tmp = (v);                                                        \
    b_new_v(&_tmp, 1);                                                         \
  })

#define p16(v)                                                                 \
  __extension__({                                                              \
    uint16_t _tmp = (v);                                                       \
    b_new_v((uint8_t *)&_tmp, 2);                                              \
  })

#define p32(v)                                                                 \
  __extension__({                                                              \
    uint32_t _tmp = (v);                                                       \
    b_new_v((uint8_t *)&_tmp, 4);                                              \
  })

#define p64(v)                                                                 \
  __extension__({                                                              \
    uint64_t _tmp = (v);                                                       \
    b_new_v((uint8_t *)&_tmp, 8);                                              \
  })

#define u8(v) (*(uint8_t *)(v)->_data)
#define u16(v) (*(uint16_t *)(v)->_data)
#define u32(v) (*(uint32_t *)(v)->_data)
#define u64(v) (*(uint64_t *)(v)->_data)
