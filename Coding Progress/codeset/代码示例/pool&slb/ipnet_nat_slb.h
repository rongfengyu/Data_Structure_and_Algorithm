#ifndef IPNET_NAT_SLB_H
#define IPNET_NAT_SLB_H


#define IPCOM_USE_CLIB_PROTO
#include <ipcom_type.h>
#include <ipcom_cstyle.h>
#include <ipcom_clib.h>
//#include <ipcom_syslog.h>
#include <ipcom_err.h> 
#include <ipcom_inet.h>
#include <rte_jhash.h>

#include <ipcom_list.h>
#include <ipcom_hash.h>
#include <ipcom_pqueue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IPNET_NAT_SCHED_LABEL_LEN  32

enum {
    DEST_F_AVAILABLE       = 0x1<<0,
    DEST_F_OVERLOAD        = 0x1<<1,
};

#define IP_VS_SH_TAB_BITS               8
#define IP_VS_SH_TAB_SIZE               (1 << IP_VS_SH_TAB_BITS)
#define IP_VS_SH_TAB_MASK               (IP_VS_SH_TAB_SIZE - 1)

/*
 * current destination pointer for weighted round-robin scheduling
 */
typedef struct Ipnet_nat_wrr_mark_struct {
    Ipcom_list *cl;   /* current list head */
    int cw;         /* current weight */
    int mw;         /* maximum weight */
    int di;         /* decreasing interval */
}
Ipnet_nat_wrr_mark;

typedef struct Ipnet_nat_sched_struct Ipnet_nat_sched;
typedef struct Ipnet_nat_service_struct Ipnet_nat_service;
typedef struct Ipnet_nat_dest_struct Ipnet_nat_dest;


typedef Ipnet_nat_dest* (* Ipnet_nat_sched_func) (Ipnet_nat_service *svc, Ip_u32 ipaddr);
typedef Ip_err (*init_srv_func)(Ipnet_nat_service *svc);
typedef Ip_err (*update_srv_func)(Ipnet_nat_service *svc);
typedef Ip_err (*exit_srv_func)(Ipnet_nat_service *svc);

typedef struct Ipnet_nat_sh_bucket_struct {
    Ipnet_nat_dest       *dest;          /* real server (cache) */
}
Ipnet_nat_sh_bucket;

/* scheduler */
/* RR-  WRR-  SH-  */
struct Ipnet_nat_sched_struct
{
    Ipcom_list              list_sched;  /* Keep list entry first */
    char                    sched_label[IPNET_NAT_SCHED_LABEL_LEN];
    Ipnet_nat_sched_func    sched_func;
    init_srv_func           init_func;
    update_srv_func         update_func;
    exit_srv_func           exit_func;
};


/* service */
struct Ipnet_nat_service_struct
{
    Ipcom_list  dests;  /* 策略下面的服务器组，初始时，调度指向第一个服务器 */
    Ip_u32      num_dests;
    Ip_u32      weight;     /* sum of servers weight */

    Ipnet_nat_sched *   scheduler;/* 策略使用的调度器的指针 */
    void *      sched_data;/* 保留上一次的调度信息 */
};

typedef struct Ipnet_nat_service_conf_struct {
    /* destination server address */
    char *sched_name;
}
Ipnet_nat_service_conf;

/* dest */
struct Ipnet_nat_dest_struct
{
    Ipcom_list  dest_list;  /* Keep list entry first */
    
    Ip_u32      ipaddr;
    
    volatile Ip_u16   flags;      /* dest status flags */
    Ip_u32      weight;     /* server weight */

    /* connection counters and thresholds */
    Ip_u32      actconns;   /* active connections */
    Ip_u32      inactconns; /* inactive connections */
    Ip_u32      persistconns;   /* persistent connections */
    Ip_u32      max_conn;   /* upper threshold */
    Ip_u32      min_conn;   /* lower threshold */

    Ipnet_nat_service *     svc;
    
};

typedef struct Ipnet_nat_dest_conf_struct {
    Ip_u32 policy_id;
    /* destination server address */
    Ip_u32 ipaddr;

    /* real server options */
    Ip_u16 conn_flags;    /* connection flags */
    Ip_u16 weight;     /* destination weight */

    /* thresholds for active connections */
    Ip_u32 max_conn;    /* upper threshold */
    Ip_u32 min_conn;    /* lower threshold */

    Ip_u32 health_check;
}
Ipnet_nat_dest_conf;

/* scheduler */
IP_PUBLIC Ip_err ipnet_nat_rr_init(void);
IP_PUBLIC Ip_err ipnet_nat_rr_exit(void);
IP_PUBLIC Ip_err ipnet_nat_wrr_init(void);
IP_PUBLIC Ip_err ipnet_nat_wrr_exit(void);
IP_PUBLIC Ip_err ipnet_nat_sh_init(void);
IP_PUBLIC Ip_err ipnet_nat_sh_exit(void);

/* dest */

/* service */

IP_GLOBAL void *ipnet_add_slb_service(char *sched_name);
IP_GLOBAL Ip_s32 slb_parse_file( char *filename, Ip_bool silent);
IP_GLOBAL Ip_err ipnet_nat_print_slb_serverlist(Ip_u32 id);
IP_GLOBAL Ip_err ipnet_nat_print_slb_schedlist(void);
IP_GLOBAL void ipnet_nat_dest_free(void *service_info);
IP_GLOBAL Ip_err slb_flush_all(Ip_u32 id);


#ifdef __cplusplus
}
#endif


#endif

