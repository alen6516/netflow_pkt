#ifndef NODE_H
#define NODE_H

#include "util.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

extern struct logger_t logger;

struct node_t {
    u8 type; // 1 for icmpv4, 17 for udp
    u32 sip;
    u32 dip;
    u16 sport;
    u16 dport;
    struct node_t *next;
    u8* pdu_ptr;
} __attribute__((packed));;

#define NODE_CALLOC() (struct node_t*) calloc(1, sizeof(struct node_t))


//TODO: add struct list_t and relative APIs


static inline void show(struct node_t* curr) {
    while (curr) {
        CHECK("-------------------\n");
        switch (curr->type) {
            case 0x1:
                CHECK("type: ICMP\n");
                break;
            case 0x6:
                CHECK("type: TCP\n");
                break;
            case 0x11:
                CHECK("type: UDP\n");
                break;
            default:
                CHECK("Unknown type\n");
                return;
        }
        CHECK("sip: %x\n", htonl(curr->sip));
        CHECK("dip: %x\n", htonl(curr->dip));
        CHECK("sport: %d\n", curr->sport);
        CHECK("dport: %d\n", curr->dport);
        curr = curr->next;
    }
}

static inline int get_node_num(struct node_t* head_node) {
    int ret = 0;
    while(head_node) {
        ret++;
        head_node = head_node->next;
    }
    return ret;
}

#endif
