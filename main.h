#ifndef MAIN_H
#define MAIN_H

#include "util.h"

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


#endif
