#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

struct logger_t logger;

int init_logger(const char *log_file) {
    logger.mode = CHECK_MODE;
    logger.log_file = fopen(log_file, "w");
    return 0;
}


void _log(mode_e this_mode, const char *format, va_list arg) {
    char buf[PRE_SIZE+LOG_SIZE];

    switch (this_mode) {
        case INFO_MODE:
            snprintf(buf, PRE_SIZE+LOG_SIZE, "[%5s] %s", "INFO", format);
            break;
        case DEBUG_MODE:
            snprintf(buf, PRE_SIZE+LOG_SIZE, "[%5s] %s", "DEBUG", format);
            break;
        case CHECK_MODE:
            snprintf(buf, PRE_SIZE+LOG_SIZE, "[%5s] %s", "CHECK", format);
            break;
        case WARN_MODE:
            snprintf(buf, PRE_SIZE+LOG_SIZE, "[%5s] %s", "DEBUG", format);
            break;
        case ERROR_MODE:
            snprintf(buf, PRE_SIZE+LOG_SIZE, "[%5s] %s", "DEBUG", format);
            break;
    }

    vsnprintf(buf + PRE_SIZE, LOG_SIZE, format, arg);

    if (NULL != logger.log_file) {
        fwrite(buf, PRE_SIZE+LOG_SIZE, 1, logger.log_file);
    }

    if (this_mode >= logger.mode) {
        printf("%s", buf);
    }
}


void INFO(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(INFO_MODE, format, arg);
}

void DEBUG(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(DEBUG_MODE, format, arg);
}

void CHECK(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(CHECK_MODE, format, arg);
}


void WARN(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(WARN_MODE, format, arg);
}

void ERROR(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(ERROR_MODE, format, arg);
}
