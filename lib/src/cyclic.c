#include "cyclic.h"
#include "bytes.h"
#include "context.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool db_sequence(bytes_t alphabet, uint8_t *a, size_t n, size_t t, size_t p,
                 size_t length, bytes_t *result) {
  if (t > n) {
    if (n % p == 0) {
      bytes_t ext = b_new(p);
      if (ext._data == NULL) {
        *result = ext;
      }

      for (size_t i = 0; i < p; i++) {
        ext._data[i] = b_at(alphabet, a[i + 1]);
      }

      b_append(result, ext);
      if (b_len(*result) >= length) {

        if (b_len(*result) > length) {
          bytes_t res = *result;
          *result = b_slice(*result, 0, length - 1);
          b_destroy(res);
        }

        return false;
      }
    }
  } else {
    a[t] = a[t - p];
    if (!db_sequence(alphabet, a, n, t + 1, p, length, result))
      return false;

    for (size_t j = a[t - p] + 1; j < b_len(alphabet); j++) {
      a[t] = j;
      if (!db_sequence(alphabet, a, n, t + 1, t, length, result))
        return false;
    }
  }
  return true;
}

bytes_t de_bruijn(bytes_t alphabet, size_t n, size_t length) {
  bytes_t result = b_new(0);
  if (result._data == NULL)
    return result;

  uint8_t *a = calloc(n * b_len(alphabet), sizeof(uint8_t));
  if (!a) {
    b_destroy(result);
    return (bytes_t){._len = 0, ._data = NULL};
  }

  db_sequence(alphabet, a, n, 1, 1, length, &result);

  free(a);

  return result;
}

bytes_t cyclic(size_t length) {
  bytes_t result =
      de_bruijn(context.cyclic_alphabet, context.cyclic_size, length);
  return result;
}
