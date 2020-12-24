/*******************************************************************************
  文 件 名: ipnet_nat_slb.c
  创 建 人: zjb                
  创建日期: 2018年8月23日
  文件描述: nat slb功能支持
  其    它: 
  修改记录: 
  1. 2018年8月23日，赵俊波
   创建此文件
*******************************************************************************/

/* -------------------------- 头文件包含区 开始 ----------------------------- */

#include "ipnet_config.h"

#ifdef IPNET_USE_NAT_SLB
#include <ipnet_nat_slb.h>

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ipcom_sock2.h>

#include "ipnet_h.h"
#include "ipnet_nat.h"
#include "ipnet_nat_h.h"


/* ========================== 头文件包含区 结束 ============================= */


/* -------------------------- 调试宏定义区 开始 ----------------------------- */

// #define MATCH_DEBUG
// #define PKT_DEBUG

/* ========================== 调试宏定义区 结束 ============================= */


/* -------------------------- 本地宏定义区 开始 ----------------------------- */

#define IPNET_NAT_MAX_SCHEDULER 5  /* 支持的调度器数目 */

/* ========================== 本地宏定义区 结束 ============================= */


/* -------------------------- 本地类型定义区 开始 --------------------------- */


/* ========================== 本地类型定义区 结束 =========================== */

/* -------------------------- 外部引用声明区 开始 --------------------------- */
/* ========================== 外部引用声明区 结束 =========================== */

/* -------------------------- 本地函数原型声明区 开始 ----------------------- */
/* ========================== 本地函数原型声明区 结束 ======================= */

/* -------------------------- 全局变量定义区 开始 --------------------------- */
IP_GLOBAL Ipcom_list  head_sched; /* 调度器scheduler链表 */

/* ========================== 全局变量定义区 结束 =========================== */


/* -------------------------- 局部变量定义区 开始 --------------------------- */

/* ========================== 局部变量定义区 结束 =========================== */

/*
 ****************************************************************************
 * 10                   GLOBAL FUNCTIONS
 ****************************************************************************
 */


/*
 ****************************************************************************
 * 11                    PUBLIC FUNCTIONS
 ****************************************************************************
 */

/*
 * Round-Robin Scheduling
 */
 
IP_STATIC Ip_err ipnet_nat_rr_init_svc(Ipnet_nat_service *svc)
{
    svc->sched_data = &svc->dests;
    return IPCOM_SUCCESS;
}

IP_STATIC Ip_err ipnet_nat_rr_update_svc(Ipnet_nat_service *svc)
{
    svc->sched_data = &svc->dests;
    return IPCOM_SUCCESS;
}

IP_STATIC Ipnet_nat_dest* ipnet_nat_sched_rr(Ipnet_nat_service *svc, Ip_u32 ipaddr)
{
    Ipcom_list *p, *q;
    Ipnet_nat_dest *dest;

    p = (Ipcom_list *)svc->sched_data;//上一次调度的srv
    p = p->next;
    q = p;
    do {
        /* skip list head */
        if (q == &svc->dests) {
            q = q->next;
            continue;
        }

        dest = ipcom_list_entry(q, Ipnet_nat_dest, dest_list);
        if (!(dest->flags & DEST_F_OVERLOAD) && &dest->weight > 0)
            /* HIT */
            goto out;
        q = q->next;
    } while (q != p);

    return NULL;

      out:
    svc->sched_data = q;

    return dest;
}


/*
 * Weighted Round-Robin Scheduling
 */

/* Get the gcd of server weights */
IP_STATIC Ip_s32 ipnet_nat_wrr_gcd(Ip_s32 a, Ip_s32 b)
{
    Ip_s32 c;

    while ((c = a % b)) {
        a = b;
        b = c;
    }
    
    return b;
}

IP_STATIC Ip_s32 ipnet_nat_wrr_gcd_weight( Ipnet_nat_service *svc)
{
    Ipnet_nat_dest *dest;
    Ip_s32 weight;
    Ip_s32 g = 0;

    for (dest = IPCOM_LIST_FIRST(&svc->dests);
     dest != IP_NULL;
     dest = IPCOM_LIST_NEXT(&dest->dest_list)){
        weight = dest->weight;
        if (weight > 0) {
            if (g > 0)
                g = ipnet_nat_wrr_gcd(weight, g);
            else
                g = weight;
        }
    }
    return g ? g : 1;
}

/* Get the maximum weight of the service destinations. */
IP_STATIC Ip_s32 ipnet_nat_wrr_max_weight(Ipnet_nat_service *svc)
{
    Ipnet_nat_dest *dest;
    Ip_s32 new_weight, weight = 0;

    for (dest = IPCOM_LIST_FIRST(&svc->dests);
     dest != IP_NULL;
     dest = IPCOM_LIST_NEXT(&dest->dest_list)){
        new_weight = dest->weight;
        if (new_weight > weight)
            weight = new_weight;
    }

    return weight;
}

IP_STATIC Ip_err ipnet_nat_wrr_init_svc(Ipnet_nat_service *svc)
{
    Ipnet_nat_wrr_mark *mark;

    /* Allocate the mark variable for WRR scheduling */
    mark = ipcom_malloc(sizeof(*mark));/*分配内存地址*/
    if(mark == IP_NULL)
    {
        ipcom_printf("ipnet_nat_wrr_init_svc() :: Could not allocate memory for create wrr mark");
        return IPCOM_ERR_NO_MEMORY;
    }
    mark->cl = &svc->dests;
    mark->cw = 0;
    mark->mw = ipnet_nat_wrr_max_weight(svc);
    mark->di = ipnet_nat_wrr_gcd_weight(svc);
    svc->sched_data = mark;
    return IPCOM_SUCCESS;
}

IP_STATIC Ip_err ipnet_nat_wrr_exit_svc(Ipnet_nat_service *svc)
{
    /* Release the mark variable */
    ipcom_free(svc->sched_data);
    
    return IPCOM_SUCCESS;
}

IP_STATIC int ipnet_nat_wrr_update_svc(Ipnet_nat_service *svc)
{
    Ipnet_nat_wrr_mark *mark = svc->sched_data;

    mark->cl = &svc->dests;
    mark->mw = ipnet_nat_wrr_max_weight(svc);
    mark->di = ipnet_nat_wrr_gcd_weight(svc);
    if (mark->cw > mark->mw)
        mark->cw = 0;
    return 0;
}

IP_STATIC Ipnet_nat_dest* ipnet_nat_sched_wrr(Ipnet_nat_service *svc, Ip_u32 ipaddr)
{
    Ipnet_nat_dest *dest;
    Ipnet_nat_wrr_mark *mark = svc->sched_data;
    Ipcom_list *p;

    /*
     * This loop will always terminate, because mark->cw in (0, max_weight]
     * and at least one server has its weight equal to max_weight.
     */
    p = mark->cl;
    while (1) {
        if (mark->cl == &svc->dests)
        {
            /* it is at the head of the destination list */

            if (mark->cl == mark->cl->next) 
            {
                /* no dest entry */
                dest = NULL;
                goto out;
            }

            mark->cl = svc->dests.next;
            mark->cw -= mark->di;
            if (mark->cw <= 0)
            {
                mark->cw = mark->mw;
                /* Still zero, which means no available servers. */
                if (mark->cw == 0)
                {
                    mark->cl = &svc->dests;
                    dest = NULL;
                    goto out;
                }
            }
        }
        else
            mark->cl = mark->cl->next;

        if (mark->cl != &svc->dests) {
            /* not at the head of the list */
            dest = ipcom_list_entry(mark->cl, Ipnet_nat_dest, dest_list);
            if (!(dest->flags & DEST_F_OVERLOAD) &&
                dest->weight >= mark->cw) {
                /* got it */
                break;
            }
        }

        if (mark->cl == p && mark->cw == mark->di) {
            /* back to the start, and no dest is found.
               It is only possible when all dests are OVERLOADED */
            dest = NULL;
            goto out;
        }
    }

      out:

    return dest;
}


/*
 * Source IP address HASH(SH) Scheduling
 */
IP_STATIC inline Ip_u32 ipnet_nat_sh_hashkey(const Ip_u32 addr)
{
    Ip_u32 addr_fold = addr;

    return (ipcom_ntohl(addr_fold)*2654435761UL) & IP_VS_SH_TAB_MASK;
}

IP_STATIC inline Ipnet_nat_dest *ipnet_nat_sh_get(Ipnet_nat_sh_bucket *tbl,const Ip_u32 addr)
{
    return (tbl[ipnet_nat_sh_hashkey(addr)]).dest;
}

/* Assign all the hash buckets of the specified table with the service. */
IP_STATIC Ip_s32 ipnet_nat_sh_assign(Ipnet_nat_sh_bucket *tbl, Ipnet_nat_service *svc)
{
    Ip_s32 i;
    Ipnet_nat_sh_bucket *b;
    Ipcom_list *p;
    Ipnet_nat_dest *dest;

    b = tbl;
    p = &svc->dests;

    Ip_u32 list_size = ipcom_list_length(p);
    
    for (i=0; i<IP_VS_SH_TAB_SIZE; i++)
    {
        if (list_size == 0)
            b->dest = IP_NULL;
        else
        {
            if (p == &svc->dests)
                p = p->next;

            dest = ipcom_list_entry(p, Ipnet_nat_dest, dest_list);
            b->dest = dest;

            p = p->next;
        }
        b++;
    }
    return IPCOM_SUCCESS;
}

/* Flush all the hash buckets of the specified table. */
IP_STATIC void ipnet_nat_sh_flush(Ipnet_nat_sh_bucket *tbl)
{
    int i;
    Ipnet_nat_sh_bucket *b;

    b = tbl;
    for (i=0; i<IP_VS_SH_TAB_SIZE; i++)
    {
        if (b->dest)
            b->dest = IP_NULL;
        b++;
    }
}

IP_STATIC Ip_err ipnet_nat_sh_init_svc(Ipnet_nat_service *svc)
{
    Ipnet_nat_sh_bucket *tbl;

    /* allocate the SH table for this service */
    tbl = ipcom_malloc(sizeof(Ipnet_nat_sh_bucket)*IP_VS_SH_TAB_SIZE);
    if (tbl == NULL) {
        ipcom_printf("%s(): no memory\n", __func__);
        return IPCOM_ERR_NO_MEMORY;
    }
    svc->sched_data = tbl;

    /* assign the hash buckets with the updated service */
    ipnet_nat_sh_assign(tbl, svc);

    return IPCOM_SUCCESS;
}

IP_STATIC Ip_err ipnet_nat_sh_exit_svc(Ipnet_nat_service *svc)
{
    Ipnet_nat_sh_bucket *tbl = svc->sched_data;
    
    /* got to clean up hash buckets here */
    ipnet_nat_sh_flush(tbl);

    /* release the table itself */
    ipcom_free(svc->sched_data);
    
    return IPCOM_SUCCESS;
}

IP_STATIC int ipnet_nat_sh_update_svc(Ipnet_nat_service *svc)
{
    Ipnet_nat_sh_bucket *tbl = svc->sched_data;

    /* got to clean up hash buckets here */
    ipnet_nat_sh_flush(tbl);

    /* assign the hash buckets with the updated service */
    ipnet_nat_sh_assign(tbl, svc);

    return IPCOM_SUCCESS;
}

IP_STATIC Ipnet_nat_dest* ipnet_nat_sched_sh(Ipnet_nat_service *svc, Ip_u32 ipaddr)
{
    Ipnet_nat_dest *dest;
    Ipnet_nat_sh_bucket *tbl;

    tbl = (Ipnet_nat_sh_bucket *)svc->sched_data;
    dest = ipnet_nat_sh_get(tbl, ipaddr);
    if(dest == IP_NULL)
        return IP_NULL;

    return dest;
}

IP_STATIC Ip_err ipnet_nat_add_sched(Ipnet_nat_sched *scheduler)
{
    Ipnet_nat_sched *nat_sched;
    Ip_err err = IPCOM_SUCCESS;

    if (scheduler->sched_label == IP_NULL || scheduler->sched_func == IP_NULL)
    {
        err = IPCOM_ERR_INVALID_ARG;
        ipcom_printf("Ipnet_nat_add_sched() :: Failed (%s)", "IPCOM_ERR_INVALID_ARG");
        return err;
    }

    if(head_sched.size >= IPNET_NAT_MAX_SCHEDULER) /* 超过支持的sched数目 */
    {
        err = IPCOM_ERR_REACHED_MAX;
        ipcom_printf("Ipnet_nat_add_sched() :: Failed (%s)", "IPCOM_ERR_REACHED_MAX");
        return err;
    }

    /* 检查是否已有相同的scheduler */
    for (nat_sched = IPCOM_LIST_FIRST(&head_sched);
         nat_sched != IP_NULL;
         nat_sched = IPCOM_LIST_NEXT(&nat_sched->list_sched))
    {
        if (ipcom_strcmp(nat_sched->sched_label, scheduler->sched_label) == 0)
            {
                err = IPCOM_ERR_DUPLICATE;
                ipcom_printf("Ipnet_nat_add_sched() :: Failed (%s): %s",
                    "IPCOM_ERR_DUPLICATE", scheduler->sched_label);
                return err;
            }
    }

    /* Allocate space for the scheduler and insert in list */
    nat_sched = ipcom_malloc(sizeof(*nat_sched));
    if(nat_sched == IP_NULL)
    {
        err = IPCOM_ERR_NO_MEMORY;
        ipcom_printf("ipnet_nat_add_sched() :: Failed (%s)", "IPCOM_ERR_NO_MEMORY");
        return err;
    }

    ipcom_memset(nat_sched, 0, sizeof(*nat_sched));
    ipcom_memcpy(nat_sched, scheduler, sizeof(*nat_sched));

    ipcom_list_insert_last(&head_sched, &nat_sched->list_sched);

    return err;
}

IP_STATIC Ip_err ipnet_nat_remove_sched(Ipnet_nat_sched *scheduler)
{
    Ipnet_nat_sched *nat_sched;
    Ip_err err = IPCOM_SUCCESS;

    if (scheduler->sched_label == IP_NULL)
    {
        err = IPCOM_ERR_INVALID_ARG;
        ipcom_printf("ipnet_nat_remove_sched() :: Failed (%s)", ipcom_err_string(err));
        return err;
    }

    /* Locate sched */
    for (nat_sched = IPCOM_LIST_FIRST(&head_sched);
         nat_sched != IP_NULL;
         nat_sched = IPCOM_LIST_NEXT(&nat_sched->list_sched))
    {
        if (ipcom_strcmp(nat_sched->sched_label, scheduler->sched_label) == 0)
            {
                break;
            }
    }

    if (nat_sched == IP_NULL)
    {
        err = IPCOM_ERR_NOT_FOUND;
        ipcom_printf("ipnet_nat_remove_sched() :: Failed (%s): %s",
                    ipcom_err_string(err), scheduler->sched_label);
        return err;
    }

    /* Remove sched and free memeory */
    ipcom_list_remove(&nat_sched->list_sched);
    ipcom_free(nat_sched);
    return err;
}

IP_STATIC Ipnet_nat_dest *ipnet_nat_lookup_dest(Ipnet_nat_service *svc, Ip_u32 *daddr)
{
    Ipnet_nat_dest *dest;

    //list_for_each_entry(dest, &svc->dests, dest_list)
    for (dest = IPCOM_LIST_FIRST(&svc->dests);
     dest != IP_NULL;
     dest = IPCOM_LIST_NEXT(&dest->dest_list)){
        if (dest->ipaddr == *daddr)
            return dest;
    }
    return NULL;

}
IP_STATIC Ip_err ipnet_nat_new_dest(Ipnet_nat_service *svc, Ipnet_nat_dest_conf *udest, Ipnet_nat_dest **dest_p)
{
    Ipnet_nat_dest *dest;


    dest = ipcom_malloc(sizeof(*dest));/*分配内存地址*/
    if(dest == IP_NULL)
    {
        ipcom_printf("ipnet_nat_new_dest() :: Could not allocate memory for create dest");
        return IPCOM_ERR_NO_MEMORY;
    }

    ipcom_memset(dest, 0, sizeof(*dest));
    dest->ipaddr = udest->ipaddr;

    dest->weight = udest->weight;
    dest->flags = udest->conn_flags;
    
    dest->actconns = 0;
    dest->inactconns = 0;
    dest->persistconns = 0;

    /* bind the service */
    if (!dest->svc) {
        dest->svc = svc;
    }

    dest->max_conn = udest->max_conn;
    dest->min_conn = udest->min_conn;

    *dest_p = dest;
    return IPCOM_SUCCESS;
}

IP_STATIC Ip_err ipnet_nat_add_dest(Ipnet_nat_service *svc, Ipnet_nat_dest_conf *udest)
{
    Ipnet_nat_dest *dest;
    Ip_u32 daddr;
    int ret;

    if (udest->weight < 0) {
        ipcom_printf("%s: server weight less than zero.\n", __func__);
        return IPCOM_ERR_NOT_SUPPORTED;
    }

    if (udest->min_conn > udest->max_conn) {
        ipcom_printf("%s: lower threshold is higher than upper threshold\n",__func__);
        return IPCOM_ERR_NOT_SUPPORTED;
    }

    daddr = udest->ipaddr;

    /* Check if the dest already exists in the list */
    dest = ipnet_nat_lookup_dest(svc, &daddr);

    if (dest != NULL) {
        ipcom_printf("%s: dest already exists.\n", __func__);
        return IPCOM_ERR_NOT_SUPPORTED;
    }

    /* Allocate and initialize the dest structure */
    ret = ipnet_nat_new_dest(svc, udest, &dest);
    if (ret) {
        return ret;
    }

    /* Add the dest entry into the list */
    ipcom_list_insert_last(&svc->dests, &dest->dest_list);//将目标服务器dest挂到service上
    svc->weight += udest->weight;
    svc->num_dests++;

    /* call the update_service function of its scheduler */
    if (svc->scheduler->update_func)
        svc->scheduler->update_func(svc);

    return IPCOM_SUCCESS;
}

/*
 *  Delete a destination (must be already unlinked from the service)
 */
IP_STATIC void __ipnet_nat_del_dest(Ipnet_nat_dest *dest)
{
    //释放dest相关指针
    dest->svc = NULL;
    ipcom_free(dest);
}

/*
 *  Unlink a destination from the given service
 */
IP_STATIC void __ipnet_nat_unlink_dest(Ipnet_nat_service *svc,        Ipnet_nat_dest *dest, int svcupd)
{
    dest->flags &= ~DEST_F_AVAILABLE;

    /* Remove it from the d-linked destination list. */
    ipcom_list_remove(&dest->dest_list);
    svc->num_dests--;

    svc->weight -= dest->weight;
#if 0
    if (svc->weight < 0) {//如果删除后权值小于0.则重新计算总权值
        Ipnet_nat_dest *tdest;
        svc->weight = 0;
        list_for_each_entry(tdest, &svc->dests, dest_list) {
            svc->weight += tdest->weight;
        }
        ipcom_printf("%s(): vs weight < 0\n", __func__);
    }
#endif
    /* Call the update_service function of its scheduler */
    if (svcupd && svc->scheduler->update_func)
        svc->scheduler->update_func(svc);
}

#if 0
IP_STATIC Ip_err ipnet_nat_del_dest(Ipnet_nat_service *svc, Ipnet_nat_dest_conf *udest)
{
    Ipnet_nat_dest *dest;

    dest = ipnet_nat_lookup_dest(svc, &udest->ipaddr);

    if (dest == NULL) {
        ipcom_printf("%s(): destination not found!\n", __func__);
        return IPCOM_ERR_FAILED;
    }

    /* Unlink dest from the service */
    __ipnet_nat_unlink_dest(svc, dest, 1);

    /* Delete the destination */
    __ipnet_nat_del_dest(dest);

    return IPCOM_SUCCESS;
}
#endif

IP_STATIC Ipnet_nat_sched *ipnet_nat_scheduler_get(const char *sched_name)
{
    Ipnet_nat_sched *sched;

    //ipcom_printf("%s(): sched_name \"%s\"\n", __func__, sched_name);

    for (sched = IPCOM_LIST_FIRST(&head_sched);
     sched != IP_NULL;
     sched = IPCOM_LIST_NEXT(&sched->list_sched)){
        if (ipcom_strcmp(sched_name, sched->sched_label) == 0) {
            /* HIT */
            return sched;
        }
    }

    return IP_NULL;
}

/*
 *  Bind a service with a scheduler
 */
IP_STATIC Ip_err ipnet_nat_bind_scheduler(Ipnet_nat_service *svc,
             Ipnet_nat_sched *scheduler)
{
    int ret;

    if (svc == IP_NULL) {
        return IPCOM_ERR_FAILED;
    }
    if (scheduler == IP_NULL) {
        return IPCOM_ERR_FAILED;
    }

    svc->scheduler = scheduler;

    if (scheduler->init_func) {
        ret = scheduler->init_func(svc);
        if (ret) {
            return ret;
        }
    }

    return IPCOM_SUCCESS;
}

/*
*  Unbind a service with its scheduler
*/
IP_STATIC Ip_err ipnet_nat_unbind_scheduler(Ipnet_nat_service *svc)
{
    Ipnet_nat_sched *sched;

    if (svc == NULL)
    {
        return IPCOM_ERR_FAILED;
    }

    sched = svc->scheduler;
    if (sched == NULL)
    {
        return IPCOM_ERR_FAILED;
    }

    svc->scheduler = IP_NULL;
    return IPCOM_SUCCESS;
}

//1.绑定调度器
//2.初始化目标服务器dest
IP_STATIC Ip_err ipnet_nat_add_service(Ipnet_nat_service_conf *u, Ipnet_nat_service **svc_p)
{
    int err, ret = 0;
    Ipnet_nat_sched *sched = NULL;
    Ipnet_nat_service *svc = NULL;

    sched = ipnet_nat_scheduler_get(u->sched_name);
    if(sched == NULL) {
        ipcom_printf("%s: scheduler not found.\n", __func__);
        return IPCOM_ERR_FAILED;
    }

    svc = ipcom_malloc(sizeof(*svc));
    if(svc == IP_NULL)
    {
        err = IPCOM_ERR_NO_MEMORY;
        ipcom_printf("ipnet_nat_add_service() :: Failed (%s)", ipcom_err_string(err));
        return err;
    }
    ipcom_memset(svc, 0, sizeof(*svc));

    ipcom_list_init(&svc->dests);

    ret = ipnet_nat_bind_scheduler(svc, sched);
    if (ret)
        goto out_err;
    sched = IP_NULL;

    *svc_p = svc;
    
    return IPCOM_SUCCESS;

out_err:
    if(svc != NULL) {
        if (svc->scheduler)
            ipnet_nat_unbind_scheduler(svc);

        ipcom_free(svc);
    }
    return ret;
}

#if 0
IP_STATIC Ip_err ipnet_nat_del_service(Ipnet_nat_service *svc)
{
    if (svc == NULL)
        return IPCOM_ERR_FAILED;

    Ipnet_nat_dest *dest;

    /* Unbind scheduler */
    ipnet_nat_unbind_scheduler(svc);

    /* Unlink the whole destination list */
    for (dest = IPCOM_LIST_FIRST(&svc->dests);
     dest != IP_NULL;
     dest = IPCOM_LIST_NEXT(&dest->dest_list)){
        __ipnet_nat_unlink_dest(svc, dest, 0);
        __ipnet_nat_del_dest(dest);
    }

    /* Free the service if nobody refers to it */
    ipcom_free(svc);

    return IPCOM_SUCCESS;
}

IP_STATIC void *ipnet_get_scheduler(char *sched_name)
{
    Ipnet_nat_sched *sched = NULL;
    sched = ipnet_nat_scheduler_get(sched_name);
    if(sched == IP_NULL)
        ipcom_printf("ipnet_get_scheduler FAILED"IP_LF);
    
    return (void *)sched;
}
#endif

IP_GLOBAL void *ipnet_add_slb_service(char *sched_name)
{
    Ipnet_nat_service *svc = IP_NULL; 
    Ipnet_nat_service_conf srvconf;
    srvconf.sched_name = sched_name;

    ipnet_nat_add_service(&srvconf, &svc);
    
    if(svc == IP_NULL)
        ipcom_printf("ipnet_add_slb_service FAILED"IP_LF);
    
    return (void *)svc;
}

IP_STATIC Ip_err slb_add_line(char *str)
{
    Ip_err   ret=IPCOM_SUCCESS;
    Ipnet_nat_rule *nat_rule;
    Ipnet_nat_dest_conf dest_conf;
    ipcom_memset(&dest_conf, 0, sizeof(dest_conf));

    char ip_str[16];
    

    if (ipcom_sscanf(str, "%d %s %d %d", &dest_conf.policy_id, ip_str, &dest_conf.weight, &dest_conf.health_check) != 4)
    {
        ret=IPCOM_ERR_FAILED;
        return ret;
    }

    dest_conf.ipaddr = ip_ntohl(ipcom_inet_addr(ip_str));

    /* Get the specified entry */
    for (nat_rule = IPCOM_LIST_FIRST(&ipnet->nat.head_rule);
         nat_rule != IP_NULL;
         nat_rule = IPCOM_LIST_NEXT(&nat_rule->list_rule))
    {
        if (0 == nat_rule->vr_index)
        {
            /* 策略ID，策略类型，dnat类型判断，防止认证策略与SLB的策略ID冲突 zjb181122 */
            if ((nat_rule->inner.rule_id ==dest_conf.policy_id) && (nat_rule->inner.rdr ==1) && (nat_rule->inner.dnat_type == 1))
                break;
        }
    }

    /* Check if the entry existed */
    if(nat_rule != IP_NULL)
    {
        if(nat_rule->sched_info != IP_NULL)
        {
            ipnet_nat_add_dest((Ipnet_nat_service *)nat_rule->sched_info, &dest_conf);
        }
        else
        {
            ipcom_printf("pls check dnat policy type, must be SLB!"IP_LF);
        }
    }

    return ret;
}


IP_GLOBAL Ip_s32 slb_parse_file( char *filename, Ip_bool silent)
{
    IP_FILE *fp;/*文件流*/  
    #define MAX_LINELENGTH  4096 //100 这个是一个限制点
    char  line[MAX_LINELENGTH];
    char *cp;
    Ip_s32 ret = -1;
    int line_num = 0;

    fp = ipcom_fopen(filename, "r");
    if(fp == IP_NULL)
    {
        if(silent == IP_FALSE)
        {
            ipcom_printf("Could not open file %s."IP_LF, filename);
        }
        ret=0;
        goto exit;
    }
    /* Read in one IP address per line in the file. */
    while(ipcom_fgets(line, MAX_LINELENGTH, fp) != IP_NULL)
    {
        line_num++;
        /* Remove carriage return */
        cp = ipcom_strchr(line, '\r');
        if(cp)
        {
            *cp = '\n';
        }
        /* Remove linefeed */
        cp = ipcom_strchr(line, '\n');
        if(cp)
        {
            *cp = '\0';
        }
        else if(ipcom_fgetc(fp) != IP_EOF)
        {
            if(silent == IP_FALSE)
            {
                ipcom_printf("Line %d is too long."IP_LF, line_num);
            }
            goto exit;
        }

        /* Remove comment */
        cp = ipcom_strchr(line, '#');
        if(cp)
        {
            *cp = '\0';
        }
        /* Skip empty and comment lines */
        if(line[0] == '\0')
        {
            continue;
        }

        /* Add rule */
        if(slb_add_line(line)!=IPCOM_SUCCESS)
        {
            ipcom_printf("Line %d is not right."IP_LF, line_num);
            ret= -1; 
            goto exit;
        }
    }
    
    ret = 0;
exit:
    if(fp != IP_NULL)
        (void)ipcom_fclose(fp);
    return ret;
}

IP_GLOBAL Ip_err ipnet_nat_print_slb_serverlist(Ip_u32 id)
{
    Ipnet_nat_rule *nat_rule;
    Ipnet_nat_service *svc = IP_NULL;
    Ipnet_nat_dest *dest = IP_NULL;
    struct Ip_in_addr ip_s;

    /* Get the specified entry */
    for (nat_rule = IPCOM_LIST_FIRST(&ipnet->nat.head_rule);
         nat_rule != IP_NULL;
         nat_rule = IPCOM_LIST_NEXT(&nat_rule->list_rule))
    {
        if (0 == nat_rule->vr_index)
        {
            /* 策略ID，策略类型，dnat类型判断，防止认证策略与SLB的策略ID冲突 zjb181122 */
            if ((nat_rule->inner.rule_id == id) && (nat_rule->inner.rdr ==1) && (nat_rule->inner.dnat_type ==1))
                break;
        }
    }
    
    /* Check if the entry existed */
    if(nat_rule != IP_NULL)
    {
        svc = nat_rule->sched_info;
        if(svc != IP_NULL)
        {
            ipcom_printf("NAT SLB server-list:\n");
            ipcom_printf("SLB policy id(DNAT): %d\n", id);
            ipcom_printf("Num of dests: %d\n", svc->num_dests);
            ipcom_printf("Sum of weight: %d\n", svc->weight);
            ipcom_printf("SLB Metric: %s\n", svc->scheduler->sched_label);
            ipcom_printf("sched_data->next: %p\n", ((Ipcom_list *)svc->sched_data)->next);
            
            dest = IPCOM_LIST_FIRST(&svc->dests);
            while(dest != IP_NULL)
            {
                /*打印dest内存地址*/
                ipcom_printf("Address: %p\n", dest);

                /*打印dest中的元素*/
                ip_s.s_addr = ip_htonl(dest->ipaddr);
                ipcom_printf("ip: %s"IP_LF, ipcom_inet_ntoa(ip_s));

                ipcom_printf("weight: %d"IP_LF, dest->weight);

                ipcom_printf("\n");
                
                dest = IPCOM_LIST_NEXT(&dest->dest_list);
            }
        }
        else
            printf("no server find!"IP_LF);
    }
    else
        printf("no rule [%d] find!"IP_LF, id);

    return 0;
}

IP_GLOBAL Ip_err ipnet_nat_print_slb_schedlist(void)
{
    Ipnet_nat_sched *nat_sched;

    ipcom_printf("NAT SLB scheduler-list:\n");

    /* 遍历scheduler链表 */
    for (nat_sched = IPCOM_LIST_FIRST(&head_sched);
         nat_sched != IP_NULL;
         nat_sched = IPCOM_LIST_NEXT(&nat_sched->list_sched))
    {

        ipcom_printf("Sched label: %s"IP_LF, nat_sched->sched_label);
        ipcom_printf("Sched Addr: %p"IP_LF, nat_sched->sched_func);
    }

    return 0;
}

//当规则有关联的dest时，则删除这些dest，并释放
IP_GLOBAL void ipnet_nat_dest_free(void *service_info)
{
    Ipnet_nat_service *svc = IP_NULL;
    Ipnet_nat_dest *dest = IP_NULL;
    int cont_free=0;
    
    if(service_info != IP_NULL)
        svc = (Ipnet_nat_service *)service_info;

    dest = IPCOM_LIST_FIRST(&svc->dests);
    while(dest != IP_NULL)
    {
        cont_free++;
        
        /* Remove proxy and free memeory */
        ipcom_list_remove(&dest->dest_list);
        ipcom_free(dest);
        
        dest = IPCOM_LIST_FIRST(&svc->dests);//注意此处是删除链表所有节点
    }

    //printf("ipnet_nat_sched_free():: cont_free = %d, svc->num_dests = %d"IP_LF, cont_free, svc->num_dests);
    
}

IP_GLOBAL Ip_err slb_flush_all(Ip_u32 id)
{
    Ipnet_nat_rule *nat_rule;
    Ipnet_nat_service *svc = IP_NULL;
    Ipnet_nat_dest *dest = IP_NULL;

    /* Get the specified entry */
    for (nat_rule = IPCOM_LIST_FIRST(&ipnet->nat.head_rule);
         nat_rule != IP_NULL;
         nat_rule = IPCOM_LIST_NEXT(&nat_rule->list_rule))
    {
        if (0 == nat_rule->vr_index)
        {
            if ((nat_rule->inner.rule_id == id) && (nat_rule->inner.rdr ==1))
                break;
        }
    }

    /* Check if the entry existed */
    if(nat_rule != IP_NULL)
    {
        svc = nat_rule->sched_info;
        
        if(svc != IP_NULL)
        {
            for (dest = IPCOM_LIST_FIRST(&svc->dests);
            dest != IP_NULL;
            dest = IPCOM_LIST_NEXT(&svc->dests))
            {
                /* Unlink dest from the service */
                 __ipnet_nat_unlink_dest(svc, dest, 1);

                /* Delete the destination */
                __ipnet_nat_del_dest(dest);
            }
        }
        else
            printf("no server find!"IP_LF);
    }
    else
        printf("no rule [%d] find!"IP_LF, id);

    return 0;
}

/* RR */
IP_STATIC Ipnet_nat_sched Ipnet_nat_rr_scheduler = 
{
    .sched_label = "rr",
    .sched_func  = ipnet_nat_sched_rr,
    .init_func   = ipnet_nat_rr_init_svc,
    .update_func = ipnet_nat_rr_update_svc,
};

IP_PUBLIC Ip_err ipnet_nat_rr_init(void)
{
    return ipnet_nat_add_sched(&Ipnet_nat_rr_scheduler);
}

IP_PUBLIC Ip_err ipnet_nat_rr_exit(void)
{
    return ipnet_nat_remove_sched(&Ipnet_nat_rr_scheduler);
}

/* WRR */
IP_STATIC Ipnet_nat_sched Ipnet_nat_wrr_scheduler = 
{
    .sched_label = "wrr",
    .sched_func  = ipnet_nat_sched_wrr,
    .init_func   = ipnet_nat_wrr_init_svc,
    .update_func = ipnet_nat_wrr_update_svc,
    .exit_func   = ipnet_nat_wrr_exit_svc,
};

IP_PUBLIC Ip_err ipnet_nat_wrr_init(void)
{
    return ipnet_nat_add_sched(&Ipnet_nat_wrr_scheduler);
}

IP_PUBLIC Ip_err ipnet_nat_wrr_exit(void)
{
    return ipnet_nat_remove_sched(&Ipnet_nat_wrr_scheduler);
}

/* SH */
IP_STATIC Ipnet_nat_sched Ipnet_nat_sh_scheduler = 
{
    .sched_label = "sh",
    .sched_func  = ipnet_nat_sched_sh,
    .init_func   = ipnet_nat_sh_init_svc,
    .update_func = ipnet_nat_sh_update_svc,
    .exit_func   = ipnet_nat_sh_exit_svc,
};

IP_PUBLIC Ip_err ipnet_nat_sh_init(void)
{
    return ipnet_nat_add_sched(&Ipnet_nat_sh_scheduler);
}

IP_PUBLIC Ip_err ipnet_nat_sh_exit(void)
{
    return ipnet_nat_remove_sched(&Ipnet_nat_sh_scheduler);
}

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
#endif    /* #ifdef IPNET_USE_NAT_SLB */


