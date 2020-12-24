#ifndef _LOG_BASE_API_H_
#define _LOG_BASE_API_H_

#include <time.h>
#include <stdlib.h>
#include <wfwlogger/libini.h>
#include <mysql/mysql.h>
#include "shmem2DB.h"

#include <signal.h>
#include <syslog.h>

/*
 *宏定义变量
 *
 */

#define DB_NAME "WFGN"

//SQL查询返回信息宏定义
#define	SQL_MAX_ROW_LEN              1024
#define	SQL_MAX_ROW_LEN_LIMIT        (SQL_MAX_ROW_LEN*0.9)
#define SQL_MAX_COLUMN_LEN           64
#define SQL_MAX_STRING_LEN           2048

//SQL语句长度
#define SQL_SCRIPT_MAX_LEN           4096

//merge表建表语句长度，表数量过多时，会比较长
#define MERGE_SQL_MAX_LEN            SQL_SCRIPT_MAX_LEN

//功能限制宏定义
#define LOG_BACKUP_LOGGATHER_PID_FILE         "/var/run/SecHome/loggather_1.pid"

#define LOG_BACKUP_CONFIG_INI_FILE            "/etc/security/SecHome/log_backup.ini"
#define LOG_BACKUP_THREAD_CHECK_INTERVAL      20

#define DISK_FILE_SYSTEM_PATH                 "/mnt/esg5000"
#define DISK_CAPACITY_CHECK_PERCENT           50
#define DISK_CAPACITY_WARN_PERCENT            80


#define DISK_CAPACITY_UPPER_LIMIT_ACTION_REMOVE    0
#define DISK_CAPACITY_UPPER_LIMIT_ACTION_STOP      1

//日志最长保存时间
#define LOG_BACKUP_MAX_TABLE_RETAIN_TIME           (7*24*60*60)
//日志表最长备份间隔
#define LOG_BACKUP_TABLE_BACKUP_INTERVAL           (24*60*60)
//日志表单表最大行数
#define LOG_BACKUP_TABLE_ROW_MAX                   (100*10000)

#define LOG_BACKUP_MAX_STRING_LEN       512

//日志类型定义
typedef enum
{
    IPF_LOG = 0,
    DDOS_DOS,
    URL_LOG,
    CONN_LOG,

    IPS_LOG,
    ANTIVIRSUS_LOG,
    CONTFLT_LOG,
    FILEIDEN_LOG,
    MAILFLT_LOG,

    ADMIN_LOG,
    DEV_LOG,
    HA_LOG,
    WDOG_LOG,

    FLOW_STAT_HISTORY,
    FLOW_STAT_HISTORY_1,
    FLOW_STAT_HISTORY_12,
    
    LOGTYPE_MAX
}log_type;

//结构体定义
#define LOG_NAME_LEN  64
typedef struct fw_log_t
{
    int         enable_merge;//是否需要统一merge分表
    log_type    logtype;
    char        log_prefix[LOG_NAME_LEN];

    char        log_base[SQL_SCRIPT_MAX_LEN];//基础日志库表创建语句
    char        log_merge_base[SQL_SCRIPT_MAX_LEN];//MERGE表创建语句前缀

    char        log_cnname[LOG_NAME_LEN];
}fw_log;

typedef struct log_syslog_msg_t
{
    char    class[LOG_BACKUP_MAX_STRING_LEN];
    int    msgtype;
    int    subtype;
    int    lastm;
    int    severity;
    char    means[LOG_BACKUP_MAX_STRING_LEN];
    char    msg[LOG_BACKUP_MAX_STRING_LEN];

}log_syslog_msg;


struct disk_infomation{
    char disk_format[LOG_BACKUP_MAX_STRING_LEN];
    char disk_total_capacity[LOG_BACKUP_MAX_STRING_LEN];
    char disk_free_capacity[LOG_BACKUP_MAX_STRING_LEN];
	char disk_usage_capacity[LOG_BACKUP_MAX_STRING_LEN];
    char disk_permissions[LOG_BACKUP_MAX_STRING_LEN];
};

struct log_backup_conf_t{
    char disk_file_system_path[LOG_BACKUP_MAX_STRING_LEN];
    int  disk_capacity_warning_percent;
    int  disk_capacity_check_percent;
    int  disk_capacity_limit_action;
    int  sql_table_retain_time_max;
    int  sql_table_backup_interval;
    int  sql_table_row_max;
};

//函数声明
int disk_capacity_print(const char * path);
int disk_capacity_check(const char * path,int percent);
int log_backup_config_load(const char* inifile);
int log_backup_get_log_num();
int log_merge_sql_init();
time_t log_backup_del_earliest_day();
int log_backup_del_time_exceed();
int log_backup_backup(int interval);

void log_backup_syslog();

void stop_logging();
void start_logging();

#endif//_LOG_BASE_API_H_

