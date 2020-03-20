#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "util.h"

#define LOGGER_FILE "./log.txt"
#define PRE_SIZE 9      // "[WARN] "
#define LOG_SIZE 100    // msg body 


typedef enum {
    INFO_MODE = 0,
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


int init_logger(const char *);

void _log(mode_e this_mode, const char *format, va_list arg);


void INFO(const char *format, ...);
void DEBUG(const char *format, ...);
void CHECK(const char *format, ...);
void WARN(const char *format, ...);
void ERROR(const char *format, ...);

/*
int main () {
    g_mode = info_mode;
    INFO("123 %c %d %p\n", 'v', 87, NULL);
    
    const char *s = "12321";
    printf("%s\n", s);
}
*/
#endif
