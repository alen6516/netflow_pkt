#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;


typedef enum {
    DEFAULT_FAIL = 1,
    MALLOC_FAIL,
    PARSE_ARG_FAIL,
    CONNECT_FAIL
} fail_e;


static inline void err_exit(fail_e reason)
{
    switch (reason) {
        case DEFAULT_FAIL:
            //printf("default fail, exit\n");
            break;
        case MALLOC_FAIL:
            printf("malloc fail, exit\n");
            break;
        case PARSE_ARG_FAIL:
            printf("parse arg fail, exit\n");
            printf("use '-i 20.20.20.1' or '-u/-t 20.20.20.1 8000'\n");
            break;
        case CONNECT_FAIL:
            printf("connect fail, exit\n");
            break;
        default:
            printf("no such fail code, exit\n");
            break;
    }
    exit(1);
}

#endif
