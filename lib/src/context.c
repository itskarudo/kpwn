#include "context.h"

context_t context = {
    .log_level = LOG_INFO,
    .cyclic_alphabet = {._len = 26,
                        ._data = (uint8_t *)"abcdefghijklmnopqrstuvwxyz"},
    .cyclic_size = 4};
