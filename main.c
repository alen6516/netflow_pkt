#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>              /* srand(time(0)) */

#include <sys/socket.h>        /* socket(), bind(), listen(), ... */
#include <netinet/in.h>        /* AF_INET, AF_INET6 addr family and their corresponding protocol family PF_INET, PFINET6 */
#include <arpa/inet.h>         /* hton(), inet_ntop() */
#include <unistd.h>            /* read(), write(), close(), sleep() */


#include "util.h"
#include "list.h"
#include "config.h"
#include "main.h"
#include "logger.h"

struct g_var_t g_var = {
    .interval = 1,
    .send_count = 1,
};

struct logger_t logger;
struct node_t* head_node;

static inline void handle_argv(int argc, char **argv) 
{

    /*
     * -u   20.20.20.1 8787
     * -i   20.20.20.1
     * -110 20.20.20.1
     */

    /*
     * -u 20.20.101.1 -p 8787 -a 20.20.20.1 -c 10 -i 1
     *      -c and -i means the end arg
     *      -a effects the dst ip in the front of it
     */

    head_node = NODE_CALLOC();
    if (NULL == head_node) {
        err_exit(MALLOC_FAIL);
    }

    if (argc == 1) {
        // no any arg
        head_node->type = 0x1;
        head_node->sip = SRC_IP;
        head_node->dip = DST_IP;
        return;
    }

    struct node_t* prev = NULL;
    struct node_t* curr = head_node;
    int i = 1;
    int ret = 1;    // success
    fail_e fail_code = DEFAULT_FAIL;
    while (i < argc) {

        if (0 == strcmp("-i", argv[i]) && i+1 < argc) {
            if (curr->dip) {
                goto add_node;
            }
            curr->type = 0x1;
            curr->sip = SRC_IP;
            ret = inet_pton(AF_INET, argv[i+1], &curr->dip);
            i += 2;

        } else if (0 == strcmp("-u", argv[i]) && i+1 < argc) {
            if (curr->dip) {
                goto add_node;
            }
            curr->type = 0x11;
            curr->sip = SRC_IP;
            ret = inet_pton(AF_INET, argv[i+1], &curr->dip);
            curr->sport = SRC_PORT;
            curr->dport = strtol(argv[i+2], NULL, 10);
            i += 3;

        } else if (0 == strcmp("-t", argv[i]) && i+1 < argc) {
            if (curr->dip) {
               goto add_node;
            }
            curr->type = 0x6;
            curr->sip = SRC_IP;
            ret = inet_pton(AF_INET, argv[i+1], &curr->dip);
            curr->sport = SRC_PORT;
            curr->dport = strtol(argv[i+2], NULL, 10);
            i += 3;

        } else if (0 == strcmp("-a", argv[i]) && i+1 < argc) {
            ret = inet_pton(AF_INET, argv[i+1], &curr->sip);
            i += 2;

        } else if (0 == strcmp("-c", argv[i]) && i+1 < argc) {
            g_var.send_count = (u32) strtol(argv[i+1], NULL, 10);
            if (!g_var.send_count) {
                ERROR(LOGGER, "send_count == 0\n");
                ret = 0;
            }
            i += 2;

        } else if (0 == strcmp("-I", argv[i]) && i+1 < argc) {
            g_var.interval = (u32) strtol(argv[i+1], NULL, 10);
            if (g_var.interval == 0) {
                ERROR(LOGGER, "interval == 0\n");
                ret = 0;
            }
            i += 2;

        } else {
            ERROR(LOGGER, "unknown option \"%s\"\n", argv[i]);
            ret = 0;
        }

        if (ret == 0) {
            fail_code = PARSE_ARG_FAIL;
            goto err;
        } else {
            continue;
        }

add_node:
        if (!curr->sip) {
            curr->sip = SRC_IP;
        }

        prev = curr;
        curr = NODE_CALLOC();
        if (NULL == curr) {
            fail_code = MALLOC_FAIL;
            goto err;
        }
        prev->next = curr;
    } 

    //list_show(LOGGER, head_node);
    show_g_var();
    return;

err:
    err_exit(fail_code);
}

static inline int make_nflow_hdr(u8** msg) 
{
    struct nflow_hdr_t* nflow_hdr;
    int ret_len = (int) sizeof(struct nflow_hdr_t);
    nflow_hdr = (struct nflow_hdr_t*) calloc(1, ret_len);
    if (NULL == nflow_hdr) err_exit(MALLOC_FAIL);
    nflow_hdr->version = htons(5);
    nflow_hdr->count = htons(get_node_num(head_node));
    nflow_hdr->sys_uptime = htonl(0x03e8);
    nflow_hdr->curr_sec = htonl(0x5dc38193);
    nflow_hdr->curr_nsec = htonl(0x226c5278);
    nflow_hdr->flow_seq = htonl(1);
    nflow_hdr->engine_type = 1;
    nflow_hdr->engine_id = 0;
    nflow_hdr->sampling_mode = 0;
    nflow_hdr->sample_rate = 0;

    *msg = (u8*) nflow_hdr;
    return ret_len;
}

static inline int make_pdu(u8** msg, struct node_t* node) 
{
    int ret_len = (int) sizeof(struct pdu_t);
    struct pdu_t* pdu;
    pdu = (struct pdu_t*) calloc(1, ret_len);
    if (NULL == pdu) err_exit(MALLOC_FAIL);
    pdu->src_addr = node->sip;
    pdu->dst_addr = node->dip;
    pdu->next_hop = htonl(DST_IP);
    pdu->input_intf = htons(0);
    pdu->output_intf = htons(0);
    pdu->pkt_count = htonl(1001);
    pdu->octet_count = htonl(2002);
    pdu->start_time = htonl(0x01b6);
    pdu->end_time = htonl(0x023b);
    pdu->src_port = htons(node->sport);
    pdu->dst_port = htons(node->dport);
    pdu->tcp_flag = 0;
    pdu->protocol = node->type;
    pdu->ip_tos = 0;
    pdu->src_as = htons(0x44cc);
    pdu->dst_as = htons(0x3725);
    pdu->src_mask = 12;
    pdu->dst_mask = 21;

    *msg = (u8*) pdu;
    return ret_len;
}

// main caller
static inline int make_nflow_pkt(u8 **msg) 
{
    u8* pdu;
    struct node_t* curr_node;
    curr_node = head_node;

    int total_pdu_len = 0;

    while (curr_node) {
        total_pdu_len += make_pdu(&pdu, curr_node);
        curr_node->pdu_ptr = pdu;
        curr_node = curr_node->next;
    }
    
    // make nflow hdr
    int nflow_hdr_len = 0;
    u8* nflow_hdr;
    nflow_hdr_len = make_nflow_hdr(&nflow_hdr);
    
    // make nflow pkt
    int nflow_pkt_len = nflow_hdr_len + total_pdu_len;
    u8* ret = (u8*) calloc(1, nflow_pkt_len);
    if (NULL == ret) err_exit(MALLOC_FAIL);
    int curr_len = 0;
    memcpy(ret, nflow_hdr, nflow_hdr_len);
    curr_len += nflow_hdr_len;

    // append pdu one by one
    curr_node = head_node;
    while (curr_node) {
        memcpy(ret+curr_len, curr_node->pdu_ptr, sizeof(struct pdu_t));
        curr_len += sizeof(struct pdu_t);
        curr_node = curr_node->next;
    }

    assert(curr_len == nflow_pkt_len);
    *msg = ret;
    return curr_len;
}

int main (int argc, char *argv[]) 
{
    init_logger(&logger, LOGGER_FILE);
    g_var.logger = &logger;

    handle_argv(argc, argv);
    g_var.head_node = head_node;

    srand(time(0));
    u8 *msg;
    int len = make_nflow_pkt(&msg);

    int sockfd, n;
    struct sockaddr_in serv_addr;

    // init
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(COLLECTOR_IP);
    serv_addr.sin_port = htons(NETFLOW_PORT);
    serv_addr.sin_family = AF_INET;

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        err_exit(CONNECT_FAIL);
    }

    for (int t=0; t < g_var.send_count; t++) {
        if (t != 0) {
            sleep(g_var.interval);
        }
        list_show(LOGGER, head_node);
        sendto(sockfd, (void*) msg, len, 0, (struct sockaddr*) NULL, sizeof(serv_addr));
    }
    close(sockfd);
}
