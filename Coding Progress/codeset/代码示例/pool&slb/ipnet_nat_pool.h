#ifndef IPNET_NAT_POOL_H
#define IPNET_NAT_POOL_H


#define IPCOM_USE_CLIB_PROTO
#include <ipcom_type.h>
#include <ipcom_cstyle.h>
#include <ipcom_clib.h>
#include <ipcom_syslog.h>
#include <ipcom_err.h> 
#include <ipcom_inet.h>
#include <rte_jhash.h>

#include <ipcom_list.h>
#include <ipcom_hash.h>
#include <ipcom_pqueue.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    HA_MODE_ROUTE = 0,
    HA_MODE_BRIDGE
}ha_mode;

typedef enum
{
    HA_STATUS_FAULT = 0,
    HA_STATUS_MASTER,
    HA_STATUS_BACKUP
}ha_status;

typedef struct ha_info_struct
{
    ha_mode      hamode;
    ha_status    hastatus;
}
ha_info;


typedef struct Ipport_list_struct
{
    Ip_u32      ip;
    Ip_u32     *ports;
    Ip_u16      last_port;
    Ip_u16      used_port_cnt;
}
Ipport_list;



/*
 *===========================================================================
 *                              Ippool_list
 *===========================================================================
 */
typedef struct Ippool_list_struct
{
    Ipcom_list  list_ippool;

    Ip_u8       pool_type;/* 0-SNAT pool; 1-DNAT pool */
    Ip_u32      pool_id;

    Ip_u32      min_ip;
    Ip_u32      max_ip;
    Ip_u32      ip_cnt;

    Ip_u16      min_port;
    Ip_u16      max_port;
    Ip_u16      port_cnt;

    Ipport_list *ip_array;

    Ip_u8       delflag; /*1,删除，0，不删*/
    Ip_u8       status;  /*1,有效，0，无效*/
}
Ippool_list;

IP_GLOBAL void ippool_print_all(void);
IP_GLOBAL Ip_err Ipnet_add_natpool(Ip_u8 type, Ip_u32 id, char *str);
IP_GLOBAL Ip_err Ipnet_del_natpool(Ip_u8 type, Ip_u32 id);
IP_GLOBAL Ip_err Ipnet_ha_info(ha_mode mode, ha_status status);

IP_GLOBAL Ip_s32 ippool_parse_file( char *filename, Ip_bool silent);

IP_GLOBAL void ippool_flush_all(void);
IP_GLOBAL void ippool_flush_type(Ip_u8 type);

IP_GLOBAL void* ipnet_get_natpool(Ip_u8 type, Ip_u32 id);
IP_GLOBAL Ip_bool Ipnet_test_natpool(Ip_u32 ip_in);

#ifdef __cplusplus
}
#endif


#endif
