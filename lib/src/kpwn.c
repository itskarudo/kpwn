#include "context.h"
#include <gc/gc.h>

context_t context = {
    .log_level = LOG_INFO, .cyclic_alphabet = NULL, .cyclic_size = 4};

static void init_context(void) {
  context.cyclic_alphabet = b("abcdefghijklmnopqrstuvwxyz");
}

static __attribute__((constructor)) void kpwn_init(void) {
  GC_INIT();
  init_context();
}
