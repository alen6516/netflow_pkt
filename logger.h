#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "util.h"

#define LOGGER_FILE "./log.txt"
#define PRE_SIZE 9      // "[WARN] "
#define LOG_SIZE 100     // msg body 

typedef enum {
    QUIET_MODE = 0,
    INFO_MODE,
    DEBUG_MODE,
    CHECK_MODE,
    WARN_MODE,
    ERROR_MODE
} mode_e;


struct logger_t {
    mode_e mode;
    FILE* log_file;
};

#define LOGGER_CALLOC() (struct logger_t*)calloc(1, sizeof(struct logger_t))


int init_logger(struct logger_t *, const char *);

void _log(struct logger_t *, mode_e, const char *, va_list);


void INFO(struct logger_t *, const char *, ...);
void DEBUG(struct logger_t *, const char *, ...);
void CHECK(struct logger_t *, const char *, ...);
void WARN(struct logger_t *, const char *, ...);
void ERROR(struct logger_t *, const char *, ...);

/*
int main () {
    g_mode = info_mode;
    INFO("123 %c %d %p\n", 'v', 87, NULL);
    
    const char *s = "12321";
    printf("%s\n", s);
}
*/
#endif
