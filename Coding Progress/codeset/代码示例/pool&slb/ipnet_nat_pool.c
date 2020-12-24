/*******************************************************************************
  文 件 名: ipnet_nat_pool.c
  创 建 人: zjb                
  创建日期: 2018年4月8日
  文件描述: nat pool地址池功能支持
  其    它: 
  修改记录: 
  1. 2018年4月8日，赵俊波
   创建此文件
*******************************************************************************/

/* -------------------------- 头文件包含区 开始 ----------------------------- */



#define IPNET_USE_NAT_POOL

#ifdef IPNET_USE_NAT_POOL
#include <ipnet_nat_pool.h>

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ipcom_sock2.h>
#include "ipnet_nat.h"

/* ========================== 头文件包含区 结束 ============================= */


/* -------------------------- 调试宏定义区 开始 ----------------------------- */

// #define MATCH_DEBUG
// #define PKT_DEBUG

/* ========================== 调试宏定义区 结束 ============================= */


/* -------------------------- 本地宏定义区 开始 ----------------------------- */
#define IPFIREWALL_IPSET_MAX_ID   32
#define IP_SET_MAX_BINDINGS 6
#define IP_SET_MAX_SIZE 20

#define LIST_OPEN '['
#define LIST_CLOSE ']'

#define NAT_START_PORT 1024
#define NAT_END_PORT 65535

const char *ha_mode_array[]   = {"HA_MODE_ROUTE",  "HA_MODE_BRIDGE"};
const char *ha_status_array[] = {"HA_STATUS_FAULT","HA_STATUS_MASTER","HA_STATUS_BACKUP"};

/* ========================== 本地宏定义区 结束 ============================= */


/* -------------------------- 本地类型定义区 开始 --------------------------- */


/* ========================== 本地类型定义区 结束 =========================== */

/* -------------------------- 外部引用声明区 开始 --------------------------- */
/* ========================== 外部引用声明区 结束 =========================== */

/* -------------------------- 本地函数原型声明区 开始 ----------------------- */
/* ========================== 本地函数原型声明区 结束 ======================= */

/* -------------------------- 全局变量定义区 开始 --------------------------- */
IP_GLOBAL Ipcom_list  head_ippool;
IP_GLOBAL Ipcom_list  head_dnatpool;
IP_GLOBAL ha_info natpool_ha;

/* ========================== 全局变量定义区 结束 =========================== */

/* -------------------------- 局部变量定义区 开始 --------------------------- */

/* ========================== 局部变量定义区 结束 =========================== */



/*******************************************************************************
  函数名称: ippool_find
  函数描述: 查找ippool是否存在
  参	 数: id: ippool名称,即pool的数据库id
          type: pool类型，0-SNAT pool 1-DNAT pool
  返 回 值: 集合指针。
  历史记录:

*******************************************************************************/
IP_STATIC Ippool_list *
ippool_find(Ip_u8 type, Ip_u32 id)
{
    Ippool_list *ip_pool = IP_NULL;

    /* Check input */
    if (id == IP_NULL)
    {
        return IP_NULL;
    }

    if (type == 0)/* SNAT pool */
        ip_pool = IPCOM_LIST_FIRST(&head_ippool);
    else/* DNAT pool */
        ip_pool = IPCOM_LIST_FIRST(&head_dnatpool);

    while(ip_pool != IP_NULL)
    {
        if((ip_pool->pool_id == id)&&(ip_pool->pool_type == type))
        {
            return ip_pool;
        }
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }
    return IP_NULL;
}

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

/*******************************************************************************
  函数名称: ippool_print_all
  函数描述: 打印pool中的所有元素
  参	 数:	  
  返 回 值:	 1 = ok.
	         0 = failed.
  历史记录:

*******************************************************************************/
IP_GLOBAL void 
ippool_print_all(void)
{
    struct Ip_in_addr maxip, minip;
    Ippool_list *ip_pool;
    ipcom_printf("NAT POOL TABLES:\n");
    ip_pool = IPCOM_LIST_FIRST(&head_ippool);
    ipcom_printf("\nSNAT pool:\n");
    while(ip_pool != IP_NULL)
    {
        /*打印pool名*/
        ipcom_printf("Name: %d, Address: %p\n", ip_pool->pool_id, ip_pool);

        /*打印pool中的元素*/
        maxip.s_addr = ip_htonl(ip_pool->max_ip);
        minip.s_addr = ip_htonl(ip_pool->min_ip);
        ipcom_printf("ip: %s-", ipcom_inet_ntoa(minip));/* 不能使用ipcom_inet_ntoa连续打印两个ip，可能和wr的实现有关 */
        ipcom_printf("%s\n", ipcom_inet_ntoa(maxip));
        ipcom_printf("ip_cnt: %d\n", ip_pool->ip_cnt);

        ipcom_printf("port: %d-%d\n", ip_pool->min_port, ip_pool->max_port);
        ipcom_printf("port_cnt: %d\n", ip_pool->port_cnt);

        ipcom_printf("\n");
        
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }

    ip_pool = IPCOM_LIST_FIRST(&head_dnatpool);
    ipcom_printf("\nDNAT pool:\n");
    while(ip_pool != IP_NULL)
    {
        /*打印pool名*/
        ipcom_printf("Name: %d, Address: %p\n", ip_pool->pool_id, ip_pool);

        /*打印pool中的元素*/
        maxip.s_addr = ip_htonl(ip_pool->max_ip);
        minip.s_addr = ip_htonl(ip_pool->min_ip);
        ipcom_printf("ip: %s-", ipcom_inet_ntoa(minip));/* 不能使用ipcom_inet_ntoa连续打印两个ip，可能和wr的实现有关 */
        ipcom_printf("%s\n", ipcom_inet_ntoa(maxip));
        ipcom_printf("ip_cnt: %d\n", ip_pool->ip_cnt);

        ipcom_printf("\n");
        
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }

    ipcom_printf("HA info: mode=%s, status=%s\n", ha_mode_array[natpool_ha.hamode], ha_status_array[natpool_ha.hastatus]);
}

/*******************************************************************************
  函数名称: ippool_del
  函数描述: 删除某个pool
  参	 数:	  
  返 回 值:	  IPCOM_SUCCESS
              IPCOM_ERR_FAILED
  历史记录:

*******************************************************************************/
IP_STATIC Ip_err
ippool_del(Ip_u8 type, Ip_u32 id)
{
    Ippool_list *ip_pool = IP_NULL;
    Ip_err err = IPCOM_SUCCESS;

    Ipport_list *ip_array = IP_NULL;
    Ip_u32 i;

    /* Check input */
    if (id <= 0)
    {
        return IPCOM_ERR_FAILED;
    }
    

    if (type == 0)/* SNAT pool */
        ip_pool = IPCOM_LIST_FIRST(&head_ippool);
    else/* DNAT pool */
        ip_pool = IPCOM_LIST_FIRST(&head_dnatpool);

    while(ip_pool != IP_NULL)
    {
        if((ip_pool->pool_id == id) && (ip_pool->pool_type == type))
        {
            ipcom_list_remove(&ip_pool->list_ippool);
            ip_pool->status = 0x00;
            ip_array = ip_pool->ip_array;
            if(ip_array != IP_NULL)
            {
                for(i=0; i<ip_pool->ip_cnt; i++)
                {
                    if(ip_array[i].ports != IP_NULL)
                        ipcom_free(ip_array[i].ports);
                }

                ipcom_free(ip_array);
            }
            ipcom_free(ip_pool);
            return err;
        }
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }
    
    return IPCOM_ERR_FAILED;
}

/*******************************************************************************
  函数名称: ippool_add
  函数描述: 添加指定IP地址范围到pool中
  参	 数:	  
  返 回 值:	  IPCOM_SUCCESS
              IPCOM_ERR_FAILED
  历史记录:

*******************************************************************************/
IP_STATIC Ip_err
ippool_add(Ippool_list *ip_pool,const char *ipaddr)
{
    Ip_err err = IPCOM_SUCCESS;
    char  *hyphen_pos;/*网段*/
    Ipport_list *ip_array;
    Ip_s32 num_longs;
    Ip_u32 i;

    /* Try to parse the line as an IP address. */
    if ((hyphen_pos=strchr(ipaddr, '-')) != IP_NULL) 
    {
        /* ipv4范围 */
        *hyphen_pos = '\0';
        hyphen_pos++;

        /* 校验max_ip的合法性，不能为空 */
        if (*hyphen_pos == '\0')
        {
            err = IPCOM_ERR_FAILED;
            goto exit;
        }
        ip_pool->max_ip = ip_ntohl(ipcom_inet_addr(hyphen_pos));
        ip_pool->min_ip = ip_ntohl(ipcom_inet_addr(ipaddr));

        if(ip_pool->max_ip < ip_pool->min_ip)
        {
            ipcom_printf("low IP address.address=%s high IP address.address=%s\n",ipaddr,hyphen_pos);
            
            err = IPCOM_ERR_FAILED;
            goto exit;
        }

        /* 计算地址池IP个数 */
        ip_pool->ip_cnt = ip_pool->max_ip - ip_pool->min_ip + 1;

        ip_pool->min_port = NAT_START_PORT;
        ip_pool->max_port = NAT_END_PORT;
        ip_pool->port_cnt = ip_pool->max_port - ip_pool->min_port + 1;

        ip_array = ipcom_malloc(ip_pool->ip_cnt * sizeof(Ipport_list));
        if (ip_array == IP_NULL)
        {
            err = IPCOM_ERR_NO_MEMORY;
            goto exit;
        }
        ip_pool->ip_array = ip_array;
        for(i=0; i<ip_pool->ip_cnt; i++)
        {
            ip_array[i].ip            = ip_pool->min_ip + i;
            
            ip_array[i].used_port_cnt = 0;

            num_longs = ((ip_pool->max_port - ip_pool->min_port)>>5) + 1;
            ip_array[i].ports = ipcom_malloc(num_longs<<2);
            if(ip_array[i].ports == IP_NULL)
            {
                err = IPCOM_ERR_NO_MEMORY;
                goto exit;
            }
            ipcom_memset(ip_array[i].ports, 0, num_longs<<2);
            ip_array[i].last_port     = ip_pool->min_port - 1;/* 初始时，上一端口为 min-1 */
        }
        
        ip_pool->status = 0xFC;
    }
    else
    {
        err = IPCOM_ERR_FAILED;
    }
    
exit:
	return err;
}

IP_STATIC Ip_err
dnatpool_add(Ippool_list *ip_pool,const char *ipaddr)
{
    Ip_err err = IPCOM_SUCCESS;
    char  *hyphen_pos;/* 网段 */

    /* Try to parse the line as an IP address. */
    if ((hyphen_pos=strchr(ipaddr, '-')) != IP_NULL) 
    {
        /* ipv4范围 */
        *hyphen_pos = '\0';
        hyphen_pos++;

        /* 校验max_ip的合法性，不能为空 */
        if (*hyphen_pos == '\0')
        {
            return IPCOM_ERR_FAILED;
        }
        ip_pool->max_ip = ip_ntohl(ipcom_inet_addr(hyphen_pos));
        ip_pool->min_ip = ip_ntohl(ipcom_inet_addr(ipaddr));

        if(ip_pool->max_ip < ip_pool->min_ip)
        {
            ipcom_printf("low IP address.address=%s high IP address.address=%s\n",ipaddr,hyphen_pos);

            return IPCOM_ERR_FAILED;
        }

        /* 计算地址池IP个数 */
        ip_pool->ip_cnt = ip_pool->max_ip - ip_pool->min_ip + 1;
        
        ip_pool->status = 0xFC;
    }
    else
    {
        err = IPCOM_ERR_FAILED;
    }

	return err;
}

/*******************************************************************************
  函数名称: __ippool_strncpy
  函数描述: 拷贝n个字符串
  参	 数:	  
  返 回 值:	  IPCOM_SUCCESS
              IPCOM_ERR_FAILED
  历史记录:

*******************************************************************************/
IP_STATIC int __ippool_strncpy(char *dst, const char *src, size_t dst_size)
{
    char *ret = NULL;

    if (dst == NULL || src == NULL || dst_size <= 0)/*判断不能够缓冲区溢出*/
        return -1;

    dst[dst_size - 1] = '\0';

    ret = strncpy(dst, src, dst_size);

    /* Not sure if this ever happens but might as
     * well be on the safe side */
    if (ret == NULL)
        return -1;

    if (dst[dst_size - 1] != '\0')
    {
        /* result was truncated */
        dst[dst_size - 1] = '\0';
        return 1;
    }

    return 0;
}

IP_STATIC char *__ippool_strnup(const char *src, size_t dst_size)
{
    char *ret = ipcom_malloc(dst_size + 1);
    int ret_val;
    ret_val = __ippool_strncpy(ret, src, dst_size + 1);
    if(ret_val == -1)
    {
        ipcom_free(ret);
            return NULL;
    }
    return ret;
}

/* Support function for ipset_parse_line.  Used to
 * correctly match up end brackets.
 *  (Can't just do strchr(str, ']') because of the
 *  [a, [b], c] case, and can't do strrchr because
 *  of the [a, [b], [c]] case) */
static char *_find_end_token(char *str)
{
    int stack = 0;
    for(; *str; str++)
    {
        if(*str == LIST_OPEN)
        {
            stack++;
        }
        else if(*str == LIST_CLOSE)
        {
            stack--;
        }
        if(!stack)
        {
            return str;
        }
    }

    return NULL;
}

/*******************************************************************************
  函数名称: ippool_create
  函数描述:  1、根据ip组id值创建ippool
           2、初始化ippool
  参	 数:	   id - ippool db id.
  返 回 值:   IPCOM_SUCCESS
           IPCOM_ERR_FAILED
  历史记录:

*******************************************************************************/
IP_STATIC Ip_err
ippool_create(Ip_u8 type, Ip_u32 id)
{
    //void            *suspend_handle;
    Ippool_list     *ip_pool;
    Ip_err          ret = IPCOM_SUCCESS;

    /* Check for duplicate */
/*
    suspend_handle = ipnet_suspend_stack();
    if (suspend_handle == IP_NULL)
    {
        ipcom_printf("ippool_create():: Could not suspend stack");
        return IPCOM_ERR_FAILED;
    }
*/
    ip_pool = ipcom_malloc(sizeof(*ip_pool));/* 分配内存地址 */
    if(ip_pool == IP_NULL)
    {
        ipcom_printf("ippool_create() :: Could not allocate memory for create ippool");
        ret = IPCOM_ERR_NO_MEMORY;
        goto out;
    }

    ipcom_memset(ip_pool, 0, sizeof(*ip_pool));
    ip_pool->pool_id = id;
    ip_pool->pool_type = type;
    
    ip_pool->delflag=0;

    /* Insert the structure in list */

    if (type == 0)/* SNAT pool */
        ipcom_list_insert_last(&head_ippool, &ip_pool->list_ippool);
    else/* DNAT pool */
        ipcom_list_insert_last(&head_dnatpool, &ip_pool->list_ippool);

out:
    //ipnet_resume_stack(suspend_handle);

    return ret;
}

/*
 *===========================================================================
 *                    ippool_parse_elements
 *===========================================================================
 * Description:
 * Parameters:  id - poolname
                ptr -Parses an IP list described by 'str' and saves the results in 'var'.
 * Returns:     IPCOM_SUCCESS or error code.
 */
IP_STATIC Ip_err
ippool_parse_elements(Ip_u8 type, Ip_u32 id, char *str)
{
    //void            *suspend_handle;
    Ippool_list     *ip_pool;
    Ip_err          ret = IPCOM_SUCCESS;

    /* Find ippool  */
    /*
    suspend_handle = ipnet_suspend_stack();
    if (suspend_handle == IP_NULL)
    {
        ipcom_printf("ippool_parse_elements :: Could not suspend stack");
        return IPCOM_ERR_FAILED;
    }
    */
    ip_pool = ippool_find(type, id);
    if (ip_pool != IP_NULL)
    {
        /*这里添加实际元素*/
        if(type == 0)
            ret = ippool_add(ip_pool, str);
        else
            ret = dnatpool_add(ip_pool, str);
    }
    else
    {
        ipcom_printf("ippool_parse_elements() :: Ippool '%s' not found", id);
        ret = IPCOM_ERR_NOT_FOUND;
    }
    
    //ipnet_resume_stack(suspend_handle);
    return ret;
}

IP_GLOBAL Ip_err
Ipnet_add_natpool(Ip_u8 type, Ip_u32 id, char *str)
{
    Ippool_list *ip_pool;
     Ip_err ret=IPCOM_SUCCESS;
     Ip_u32 poolname = id;
     
    if(poolname <= 0) /* pool id 等于零是异常情况 */
    {
        ret = IPCOM_ERR_FAILED;
        goto out;
    }

    ip_pool = ippool_find(type, id);
    if (ip_pool != IP_NULL)
    {
        /*判断地址池是否已经存在，不支持添加相同id的地址池*/
        ipcom_printf("Ipnet_add_natpool() :: %s '%d' already exists!"IP_LF, type?"DNAT pool":"SNAT pool", id);
        ret = IPCOM_ERR_FAILED;
        goto out;
    }
    
    ret = ippool_create(type, poolname);/* 新建一个pool对象 */
    if(ret < 0)
    {
        ipcom_printf("Ipnet_add_natpool() :: Could not allocate memory for create ippool");
        goto out;
    }

    ret = ippool_parse_elements(type, poolname, str);/* 添加pool中内容 */
    if(ret < 0)
    {
        ipcom_printf("Ipnet_add_natpool() :: Could not add elements into ippool");
        ippool_del(type, poolname);
        goto out;
    }

out:
    return ret;
}

IP_GLOBAL Ip_err
Ipnet_del_natpool(Ip_u8 type, Ip_u32 id)
{
     Ip_err ret=IPCOM_SUCCESS;

     ret = ippool_del(type, id);
    if(ret < 0)
    {
        ipcom_printf("Ipnet_del_natpool() :: Could not del the ippool");
        goto out;
    }

out:
    return ret;
}

IP_GLOBAL Ip_err
Ipnet_ha_info(ha_mode mode, ha_status status)
{
    Ip_err ret=IPCOM_SUCCESS;

    natpool_ha.hamode   = mode;
    natpool_ha.hastatus = status;
    
    return ret;
}


//判断某个ip是否在地址池中
//联合HA状态进行地址池的ARP请求响应回应zjb191205
IP_GLOBAL Ip_bool
Ipnet_test_natpool(Ip_u32 ip_in)
{
    Ippool_list *ip_pool;
    
    /*struct Ip_in_addr ip;
    ip.s_addr = ip_htonl(ip_in);*/

    if(natpool_ha.hamode == HA_MODE_BRIDGE)
        return IP_FALSE;

    if((natpool_ha.hamode == HA_MODE_ROUTE) && (natpool_ha.hastatus != HA_STATUS_MASTER))
        return IP_FALSE;
    
    ip_pool = IPCOM_LIST_FIRST(&head_ippool);
    while(ip_pool != IP_NULL)
    {
        if(ip_in>=ip_pool->min_ip && ip_in<=ip_pool->max_ip)
        {
            /*ipcom_printf("ip: %s is in ippool %d\n", ipcom_inet_ntoa(ip), ip_pool->pool_id);*/
            return IP_TRUE;
        }
        
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }

    ip_pool = IPCOM_LIST_FIRST(&head_dnatpool);
    while(ip_pool != IP_NULL)
    {
        if(ip_in>=ip_pool->min_ip && ip_in<=ip_pool->max_ip)
        {
            /*ipcom_printf("ip: %s is in dnatpool %d\n", ipcom_inet_ntoa(ip), ip_pool->pool_id);*/
            return IP_TRUE;
        }
        
        ip_pool = IPCOM_LIST_NEXT(&ip_pool->list_ippool);
    }

    /*ipcom_printf("ip: %s is not in any natpool\n", ipcom_inet_ntoa(ip));*/
    return IP_FALSE;
}


/*******************************************************************************
  函数名称: ippool_parse_line
  函数描述:解析一行字符串
  参	 数:	   str.
  返 回 值:	  IPCOM_SUCCESS
              IPCOM_ERR_FAILED
  历史记录:

*******************************************************************************/

IP_STATIC Ip_err
ippool_parse_line(char *str)
{
    Ip_err   ret=IPCOM_SUCCESS;
    char *tmpstr = IP_NULL;
    char *token = IP_NULL;
    Ip_u32   argc = 0;
    char    *argv[32+1];
    
    char    *ptr;
    Ip_u32   poolname = 0;
    char    *end;
    Ip_u32   i = 0;
    Ip_u8    pool_type = 0;

    //int setname_duplicate=0;
    /* Split string */
    tmpstr = ipcom_strdup(str);
    if (tmpstr == IP_NULL)
    {
        ret = IPCOM_ERR_FAILED;
        
    }

    token = ipcom_strtok(tmpstr, " \t");
    while(token != IP_NULL && argc < 32)
    {
        argv[argc++] = token;
        token = ipcom_strtok(IP_NULL, " \t");
    }
    argv[argc] = IP_NULL;

    while (i < 3 && argv[i] != IP_NULL) 
    {
        switch(i)
        {
            case 0:	
                if (!strncmp(argv[i], "spool", 5))
                    pool_type = 0;
                else if (!strncmp(argv[i], "dpool", 5))
                    pool_type = 1;
                else
                {
                    ret=IPCOM_ERR_FAILED;
                }
                break;

            case 1:
                poolname = ipcom_atoi(argv[i]);
                if(poolname <= 0) /* pool id 等于零是异常情况 */
                {
                    ret = IPCOM_ERR_FAILED;
                    break; 
                }

                if (ippool_find(pool_type, poolname) != IP_NULL)
                {
                    /*判断地址池是否已经存在，不支持添加相同id的地址池*/
                    printf("ippool_parse_line() :: %s '%d' already exists!"IP_LF, pool_type?"DNAT pool":"SNAT pool", poolname);
                    ret = IPCOM_ERR_FAILED;
                    break;
                }
                
                ret=ippool_create(pool_type, poolname);/* 新建一个pool对象 */
                break;

            case 2:
                ptr=argv[i];
                if(*ptr == LIST_OPEN)
                {
                    char *list_tok;
                    /* Find end of this list */
                    if ((end = _find_end_token(ptr)) == NULL)
                    {
                        /* No trailing bracket found */
                        printf("No trailing bracket found!!!!!!\n");
                        ret=IPCOM_ERR_FAILED;
                        break;
                    }
                    ptr++;
                    list_tok = __ippool_strnup(ptr, end - ptr);
                    if(NULL != list_tok)
                    {
                        /* pool地址池中只有一个IP地址范围 */
                        ret=ippool_parse_elements(pool_type, poolname,list_tok);
                        ipcom_free(list_tok);
                    }
                    else
                    {
                        ret=IPCOM_ERR_FAILED;
                    }
                }
                else 
                {
                    ret=IPCOM_ERR_FAILED;
                }
                break;

            default:
                break;
        }
        /*结束解析的条件*/
        if(ret==IPCOM_ERR_FAILED)
        {
            break;
        }
        i++;
    }
    
    ipcom_free(tmpstr);
    return ret;
}
/*
 *===========================================================================
 *                    ippool_parse_file
 *===========================================================================
 * Description:Add  ippools from a file.
 * Parameters:
 *              filename - name of the file with the rules.
 *              silent   - IP_TRUE for no stdout output.
 * Returns:     0 = ok.
 *             -1 = failed.
 */
 /*******************************************************************************
  函数名称: ippool_parse_file
  函数描述:解析配置文件中的ippool
  参	 数:	   filename - name of the file with the ippools.
  返 回 值:	  0 = ok.
 *            -1 = failed.
  历史记录:
    pool对象的下发形式：pool ID [1.1.1.1-1.1.1.2]

*******************************************************************************/
IP_GLOBAL Ip_s32
ippool_parse_file( char *filename, Ip_bool silent)
{
    IP_FILE *fp;/*文件流*/  
    #define MAX_LINELENGTH  4096 //100 这个是一个限制点
    char  line[MAX_LINELENGTH];
    char *cp;
    Ip_s32 ret = -1;
    int line_num = 0;

    //ipset_flush_all();/*首先清空集合组然后再重新加载 lty 20170315*/
    //(void)ipset_delflag_init();
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
        if(ippool_parse_line(line)!=IPCOM_SUCCESS)
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

/*******************************************************************************
  函数名称: ippool_flush_all
  函数描述:清空ippool,用于测试pool对象功能，在实际NAT规则时未使用
  参	 数:	   filename - name of the file with the ippools.
  返 回 值:

  历史记录:

*******************************************************************************/
IP_GLOBAL void
ippool_flush_all(void)
{
    ippool_flush_type(0);//snat pool
    ippool_flush_type(1);//dnat pool

    /* HA默认为路由模式，主设备 */
    natpool_ha.hamode = HA_MODE_ROUTE;
    natpool_ha.hastatus = HA_STATUS_MASTER;
}

IP_GLOBAL void ippool_flush_type(Ip_u8 type)
{
    ip_assert(type==0 || type==1);

    Ippool_list *ip_pool = IP_NULL;
    Ipport_list *ip_array = IP_NULL;
    Ip_u32 i;

    /* Remove list by pool type*/
    if (type == 0)
    {
        for (ip_pool = IPCOM_LIST_FIRST(&head_ippool);
             ip_pool != IP_NULL;
             ip_pool = IPCOM_LIST_NEXT(&head_ippool))
        {
            ip_assert(ip_pool->pool_type == type);

            ipcom_list_remove(&ip_pool->list_ippool);
            ip_pool->status = 0x00;
            ip_array = ip_pool->ip_array;
            if(ip_array != IP_NULL)
            {
                for(i=0; i<ip_pool->ip_cnt; i++)
                {
                    if(ip_array[i].ports != IP_NULL)
                        ipcom_free(ip_array[i].ports);
                }
                ipcom_free(ip_array);
            }
            ipcom_free(ip_pool);
        }
        return IPCOM_SUCCESS;
    }
    else if ( type == 1)
    {
        for (ip_pool = IPCOM_LIST_FIRST(&head_dnatpool);
             ip_pool != IP_NULL;
             ip_pool = IPCOM_LIST_NEXT(&head_dnatpool))
        {
            ip_assert(ip_pool->pool_type == type);
            
            ipcom_list_remove(&ip_pool->list_ippool);
            ip_pool->status = 0x00;
            ip_array = ip_pool->ip_array;
            if(ip_array != IP_NULL)
            {
                for(i=0; i<ip_pool->ip_cnt; i++)
                {
                    if(ip_array[i].ports != IP_NULL)
                        ipcom_free(ip_array[i].ports);
                }
                ipcom_free(ip_array);
            }
            ipcom_free(ip_pool);
        }
        return IPCOM_SUCCESS;
    }
}

/*******************************************************************************
  函数名称: ipnet_get_natpool
  函数描述:外部接口，提供通过ippool名称获取ippool指针的接口
  参	 数:	   id - name of the ipsets.
  返 回 值:	  !NULL = ok.
 *            NULL = failed.
  历史记录:

*******************************************************************************/
IP_GLOBAL void*
ipnet_get_natpool(Ip_u8 type, Ip_u32 id)
{
    void *ret = IP_NULL;

    Ippool_list *pool_list = ippool_find(type, id);
    if(pool_list != IP_NULL)
    {
        ret = (void*)pool_list; 
        /*ipcom_printf("ipnet_get_natpool(name = %d) ret = %p"IP_LF, id, ret);*/
    }

    return ret;
}

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
#endif    /* #ifdef IPNET_USE_NAT_POOL */
