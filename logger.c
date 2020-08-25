#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

extern struct logger_t logger;

int init_logger(struct logger_t* logger, const char *log_file) {
    logger->mode = CHECK_MODE;
    logger->log_file = fopen(log_file, "w+");
    return 0;
}


void _log(struct logger_t *logger, mode_e this_mode, const char *format, va_list arg) {

    char buf[PRE_SIZE+LOG_SIZE];
    int plen = 0;

    switch (this_mode) {
        case QUIET_MODE:
            return;
        case INFO_MODE:
            snprintf(buf, PRE_SIZE, "[%5s] ", "INFO");
            break;
        case DEBUG_MODE:
            snprintf(buf, PRE_SIZE, "[%5s] ", "DEBUG");
            break;
        case CHECK_MODE:
            snprintf(buf, PRE_SIZE, "[%5s] ", "CHECK");
            break;
        case WARN_MODE:
            snprintf(buf, PRE_SIZE, "[%5s] ", "WARN");
            break;
        case ERROR_MODE:
            snprintf(buf, PRE_SIZE, "[%5s] ", "ERROR");
            break;
    }

    plen = vsnprintf(buf+PRE_SIZE-1, LOG_SIZE-1, format, arg);

    if (NULL != logger->log_file) {
        fwrite(buf, PRE_SIZE-1+plen, 1, logger->log_file);
    }

    if (this_mode >= logger->mode) {
        printf("%s", buf);
    }
}


void INFO(struct logger_t *logger, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(logger, INFO_MODE, format, arg);
}

void DEBUG(struct logger_t *logger, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(logger, DEBUG_MODE, format, arg);
}

void CHECK(struct logger_t *logger, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(logger, CHECK_MODE, format, arg);
}


void WARN(struct logger_t *logger, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(logger, WARN_MODE, format, arg);
}

void ERROR(struct logger_t *logger, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(logger, ERROR_MODE, format, arg);
}
