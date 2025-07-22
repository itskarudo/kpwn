#pragma once

enum LogLevel {
  LOG_ERROR = 0,
  LOG_WARNING = 1,
  LOG_SUCCESS = 2,
  LOG_INFO = 3,
  LOG_DEBUG = 4
};

#ifdef __cplusplus
extern "C" {
#endif

void log_error(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_success(const char *fmt, ...);
void log_debug(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
