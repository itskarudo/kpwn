#pragma once

#include "bytes.h"

#ifdef __cplusplus
extern "C" {
#endif
char *url_encode(const bytes_t *);
bytes_t *url_decode(const char *);

char *b64e(const bytes_t *);
bytes_t *b64d(const char *);

char *hex(const bytes_t *);
bytes_t *unhex(const char *);

#ifdef __cplusplus
}
#endif
