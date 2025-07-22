#pragma once

#include "bytes.h"

#ifdef __cplusplus
extern "C" {
#endif

bytes_t de_bruijn(bytes_t alphabet, size_t n, size_t length);
bytes_t cyclic(size_t length);

#ifdef __cplusplus
}
#endif
