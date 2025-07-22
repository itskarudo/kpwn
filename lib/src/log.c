#include "log.h"
#include "context.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#define RESET "\x1b[0m"

static struct {
  const char *message;
  const char *fg;
  const char *bg;
} log_level_info[] = {
    [LOG_ERROR] = {.message = "ERROR", .fg = "\x1b[0;37m", .bg = "\x1b[41m"},
    [LOG_WARNING] = {.message = "!", .fg = "\x1b[1;33m", .bg = ""},
    [LOG_SUCCESS] = {.message = "+", .fg = "\x1b[1;32m", .bg = ""},
    [LOG_INFO] = {.message = "*", .fg = "\x1b[1;34m", .bg = ""},
    [LOG_DEBUG] = {.message = "DEBUG", .fg = "\x1b[1;31m", .bg = ""},
};

static void do_log(enum LogLevel level, const char *fmt, va_list ap) {

  if (level > context.log_level)
    return;

  // allow output to be redirected without a mess
  if (isatty(fileno(stdout))) {
    printf("[%s%s%s%s] ", log_level_info[level].fg, log_level_info[level].bg,
           log_level_info[level].message, RESET);
  }

  vprintf(fmt, ap);
}

void log_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  do_log(LOG_ERROR, fmt, args);
}

void log_warning(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  do_log(LOG_WARNING, fmt, args);
}

void log_success(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  do_log(LOG_SUCCESS, fmt, args);
}

void log_info(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  do_log(LOG_INFO, fmt, args);
}

void log_debug(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  do_log(LOG_DEBUG, fmt, args);
}
