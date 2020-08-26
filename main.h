#ifndef MAIN_H
#define MAIN_H

#include "util.h"
#include "logger.h"
#include "list.h"

struct nflow_hdr_t {
    u16 version;
    u16 count;
    u32 sys_uptime;
    u32 curr_sec;
    u32 curr_nsec;
    u32 flow_seq;
    u8  engine_type;
    u8  engine_id;
    u16 sample_rate  :14,
        sampling_mode: 2;
}__attribute__((packed));


struct pdu_t {
    u32 src_addr;
    u32 dst_addr;
    u32 next_hop;
    u16 input_intf;
    u16 output_intf;
    u32 pkt_count;
    u32 octet_count;
    u32 start_time;
    u32 end_time;
    u16 src_port;
    u16 dst_port;
    u8  padding1;
    u8  tcp_flag;
    u8  protocol;
    u8  ip_tos;
    u16 src_as;
    u16 dst_as;
    u8  src_mask;
    u8  dst_mask;
    u16 padding2;
}__attribute__((packed));


static inline void handle_argv(int, char **);
static inline int make_nflow_hdr(u8 **);
static inline int make_pdu(u8 **, struct node_t*);
static inline int make_nflow_pkt(u8 **);

struct g_var_t {
    u32 interval;
    u32 send_count;
    struct logger_t* logger;
    struct node_t* head_node;
}__attribute__((packed));

extern struct g_var_t g_var;
#define LOGGER g_var.logger

static inline void show_g_var() {
    CHECK(LOGGER, "######## show g_var ########\n");
    CHECK(LOGGER, "interval = %d\n", g_var.interval);
    CHECK(LOGGER, "send_count = %d\n", g_var.send_count);
}


#endif
