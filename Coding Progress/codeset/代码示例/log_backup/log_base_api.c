/*******************************************************************************
*   * @file 
*   * @brief 
*   * @author 
*   * @date 20190716  
*******************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/vfs.h>

#include <mysql/mysql.h>
#include <shmem/inshm.h>
#include "shmem2DB.h"

#include "log_base_api.h"

//16 = 11 + 5
static fw_log ngfw_log[] =
{
    {1, IPF_LOG,              "ipf_log",              "","", "包过滤日志"},
    {1, DDOS_DOS,             "anti_dos_attack_log",  "","", "抗DoS日志"},
    {1, URL_LOG,              "url_log",              "","", "URL过滤日志"},

    {0, CONN_LOG,             "connection_log",       "","", "连接数日志"},

    {1, IPS_LOG,              "ips_log",              "","", "入侵防御日志"},
    {1, ANTIVIRSUS_LOG,       "antivirus_log",        "","", "反病毒日志"},
    {1, CONTFLT_LOG,          "content_filter_log",   "","", "内容过滤日志"},
    {1, FILEIDEN_LOG,         "file_identify_log",    "","", "文件识别日志"},
    {1, MAILFLT_LOG,          "mail_filter_log",      "","", "邮件过滤日志"},

    {1, ADMIN_LOG,            "AdminLog",             "","", "操作日志"},
    {1, DEV_LOG,              "DevLog",               "","", "设备告警日志"},
    {1, HA_LOG,               "HaLog",                "","", "双机热备日志"},
        
    {0, FLOW_STAT_HISTORY,    "flow_stat_history",    "","", "历史流量日志"},
    {0, FLOW_STAT_HISTORY_1,  "flow_stat_history_1",  "","", "历史流量日志_1"},
    {0, FLOW_STAT_HISTORY_12, "flow_stat_history_12", "","", "历史流量日志_12"},
    
    {0, WDOG_LOG,             "WdogLog",              "","", "Watchdog日志"}
};

#define LOG_TYPE_NUM ((sizeof(ngfw_log)/sizeof(fw_log)))

//全局变量定义
extern DATABASE_CONN log_backup_dbconn;
extern struct log_backup_conf_t log_backup_conf;
extern log_syslog_msg syslog_msg;

//全局mysql指针
extern MYSQL *mysql;

//全局日志记录启停标识
extern int is_stop_log;

//Loggather进程pid   由于loggather进程可能存在不断的调试情况，所以每次直接从文件读取
//extern int loggather_pid;

//静态函数声明
static int log_backup_sql_query(const char *script, long *row, long *col, char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN]);
static int log_backup_del_table(const char *table);
static void db_res_printf(int row, int col, char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN]);
static int update_merge(log_type logtype);
static int log_backup_del_one_day_log(const char *day_str);
static time_t log_backup_get_earliest_day(char *day_str);
time_t log_backup_del_earliest_day();
static int log_backup_get_log_base_date(const char * log_prefix, char *day_str);
static int log_backup_get_table_row_num(const char * table_name);
static int log_backup_get_day_log_num(const char *day_str);
static time_t log_backup_get_log_base_time(log_type logtype);
static int log_backup_base_table_backup(log_type logtype);
static int log_backup_backup_day_log();
static int log_backup_delete_table_backup_file(const char *day_str);
static int str_replace(char strRes[],const char from[], const char to[]);
static int get_loggather_pid();
static unsigned long kscale(unsigned long b, unsigned long bs);
static char *convert_size(float size, char *dest);
static int get_disk_information(const char *path, struct disk_infomation *di);

/*
 ****************************************************************************
 *                     LOCAL FUNCTIONS
 ****************************************************************************
 */



int log_backup_config_load(const char* inifile)
{
    BOOL first = TRUE;
    PINI_FILE ini;
    PKEY_VALUE key;
    char kStr[MAX_KEY_LEN];
    char vStr[MAX_VALUE_LEN];

    if(!inifile)
        return -1;

    if ((ini = openIniFile((char *)inifile)) == NULL) 
    {
        printf("Error: Unable to open %s.\n", inifile);
        return -1;
    }

    /* 读出settings域内容，返回到key链表 */
    if ((key = readSection(ini, "log_backup_config")) == NULL) 
    {
        printf("Error: Read section system:%s\n", "log_backup_config");
        return -1;
    }

    /* 读取配置参数 */
    while (getOneKey(key, kStr, MAX_KEY_LEN, vStr, MAX_VALUE_LEN, first) == 0)
    {
        if(strcasecmp(kStr, "disk_file_system_path") == 0)
        {
            memset(log_backup_conf.disk_file_system_path,0,sizeof(log_backup_conf.disk_file_system_path));
            strncpy(log_backup_conf.disk_file_system_path,vStr,strlen(vStr));
        }
        else if(strcasecmp(kStr, "disk_capacity_check_percent") == 0)
        {
            log_backup_conf.disk_capacity_check_percent = atoi(vStr);
        }
        else if(strcasecmp(kStr, "disk_capacity_warning_percent") == 0)
        {
            log_backup_conf.disk_capacity_warning_percent = atoi(vStr);
        }
        else if(strcasecmp(kStr, "sql_table_retain_time_max") == 0)
        {
            log_backup_conf.sql_table_retain_time_max = atoi(vStr);
        }
        else if(strcasecmp(kStr, "sql_table_backup_interval") == 0)
        {
            log_backup_conf.sql_table_backup_interval = atoi(vStr);
        }
        else if(strcasecmp(kStr, "sql_table_row_max") == 0)
        {
            log_backup_conf.sql_table_row_max = atoi(vStr);
        }
        else if(strcasecmp(kStr, "disk_capacity_limit_action") == 0)
        {
            log_backup_conf.disk_capacity_limit_action = atoi(vStr);
        }
        //printf("  %s = %s \n", kStr, vStr); 	
        first = FALSE;
    }

    /* 关闭INI文件 */	
    closeIniFile(ini); 

    return 0;
}

void stop_logging()
{
    is_stop_log = 1;

    int pid = 0;
    pid = get_loggather_pid();

    if(pid>0)
        kill(pid, SIGUSR2);
}

void start_logging()
{
    is_stop_log = 0;
    
    int pid = 0;
    pid = get_loggather_pid();

    if(pid>0)
        kill(pid, SIGUSR1);
}

/** 
 * this function can execute sql script and fetch back a table
@param host sql server host
@param name sql server user name
@param pwd sql server user password
@param db database name
@param port sql server port
@param script sql script to exec
@param row row count to return(not include the field)
@param col column count to return
@param data_res table to return
*/
static int log_backup_sql_query(const char *script, long *row, long *col, char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN])
{

    int row_index = 0;
    int ret = 0;
    MYSQL_RES *mysql_res = NULL;
    MYSQL_FIELD *mysql_field = NULL;	
    MYSQL_ROW mysql_row;

    //使用静态数组存储查询信息
    static char sql_data_result[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN][SQL_MAX_STRING_LEN];

    int i = 0;

    if(!script || !row || !col)
        return -1;

    if(!(ret = mysql_query(mysql,script)))
    {
        if(mysql_res = mysql_store_result(mysql))
        {
            *row = mysql_num_rows(mysql_res);
            *col = mysql_num_fields(mysql_res);

            while(mysql_field=mysql_fetch_field(mysql_res))//保存查询结果字段信息
            {
                //printf("%s\t", mysql_field->name);
                memset(sql_data_result[row_index][i],0,SQL_MAX_STRING_LEN);
                if(mysql_field->name)
                {
                    strncpy(sql_data_result[row_index][i],mysql_field->name,
                        strlen(mysql_field->name)<SQL_MAX_STRING_LEN?strlen(mysql_field->name):(SQL_MAX_STRING_LEN-1));
                    data_res[row_index][i] = sql_data_result[row_index][i];
                }
                i++;
            }

            while(mysql_row = mysql_fetch_row(mysql_res))
            {
                if(row_index++ >= SQL_MAX_ROW_LEN)//结果从data_res第二行开始存储
                    break;

                for(i=0; i<*col && i<SQL_MAX_COLUMN_LEN; i++)
                {
                    memset(sql_data_result[row_index][i], 0, SQL_MAX_STRING_LEN);
                    if(mysql_row[i])
                    {
                        strncpy(sql_data_result[row_index][i],mysql_row[i],
                            strlen(mysql_row[i])<SQL_MAX_STRING_LEN?strlen(mysql_row[i]):(SQL_MAX_STRING_LEN-1));
                        data_res[row_index][i] = sql_data_result[row_index][i];
                    }
                }
            }
        }
    }

    if(mysql_res)
        mysql_free_result(mysql_res);

    return ret;
}

/** 
 * function: delete the sql table
@param mysql	: mysql handler
@param table	: table name
@return:
	0	: success
	!0	: error code
*/
static int log_backup_del_table(const char *table)
{
    char sql_script_buf[SQL_SCRIPT_MAX_LEN] = {0};

    if(!table)
        return -1;

    snprintf(sql_script_buf,sizeof(sql_script_buf),"drop table %s;",table);	
    printf("sql=%s\n",sql_script_buf);

    if(mysql_query(mysql,sql_script_buf))
    {
        printf("mysql_query error: %s\n", mysql_error(mysql));
        return -1;
    }

    return 0;
}

//mysql语句返回信息打印
static void db_res_printf(int row, int col, char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN])
{
    printf("row = %d, col =%d\n", row, col);
    int i,j;
    for(i=0;i<row+1&&i<SQL_MAX_ROW_LEN;i++)
    {
        for(j=0;j<col&&j<SQL_MAX_COLUMN_LEN;j++)
        {
            printf("(%d,%d)###%s###\n",i,j,data_res[i][j]);
        }
    }
}

//重新生成merge表
//删除之前的、获取每一类的现存表、创建新的merge表
static int update_merge(log_type logtype)
{
    int ret;

    int i_type = 0;
    long i=0;
    long j=0;

    long row = 0;
    long col = 0;

    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    while( i_type < LOG_TYPE_NUM)
    {
        //略过不需要统一merge的表
        if(ngfw_log[i_type].enable_merge != 1)
        {
            i_type++;
            continue;
        }
        
        if(ngfw_log[i_type].logtype == logtype || logtype == LOGTYPE_MAX)
        {
            //drop old merge
            char drop_merge_table[SQL_SCRIPT_MAX_LEN] = {0};
            snprintf(drop_merge_table, sizeof(drop_merge_table), "%s_merge", ngfw_log[i_type].log_prefix);

            //由于merge的存在会影响下面的表查询，所以必须先删除，
            //或者改变merge的命名规则
            //注意：merge表初始时可能没有，因此没有检查返回值！
            log_backup_del_table(drop_merge_table);//drop table ipf_log_merge;

            //create new merge
            //1、获取该日志类型的所有表
            const char * log_table_t = "SELECT table_name FROM information_schema.tables WHERE table_schema='"DB_NAME"' AND table_type='base table' AND table_name like '";
            char log_table[SQL_SCRIPT_MAX_LEN] = {0};
            strcpy(log_table, log_table_t);
            strcat(log_table, ngfw_log[i_type].log_prefix);
            strcat(log_table, "%' order by create_time desc;");

            printf("ngfw_log[i_type].log_prefix = %s\n", ngfw_log[i_type].log_prefix);
            //snprintf(log_table, sizeof(log_table), log_table_t, ngfw_log[i_type].log_prefix);
            //printf("log_table = %s\n", log_table);
            ret = log_backup_sql_query(log_table, &row, &col, data_res);
            if(ret != 0) return -1;
            //db_res_printf(row, col, data_res);

            //2、拼接merge生成语句
            char log_table_merge[MERGE_SQL_MAX_LEN] = {0};

            strcpy(log_table_merge, ngfw_log[i_type].log_merge_base);
            strcat(log_table_merge, " UNION (");

            for(i=1; i<row+1&&i<SQL_MAX_ROW_LEN; i++)//32768
            {
                //如果merge表建表语句超过整体长度，则不继续拼接子表
                if(strlen(log_table_merge) + strlen(data_res[i][0])+10 > MERGE_SQL_MAX_LEN)
                    break;
                
                //非第一个子表后面需要添加逗号“，”
                if(i != 1)
                    strcat(log_table_merge, ",");

                strcat(log_table_merge, data_res[i][0]);
            }
            strcat(log_table_merge, ");");

            //3、执行
            //printf("log_table_merge = %s\n", log_table_merge);
            ret = log_backup_sql_query(log_table_merge, &row, &col, data_res);
            if(ret != 0) 
            {
                //printf("log_table_merge create error!\n");
                return -1;
            }
        }
        i_type++;
    }
}

void log_backup_syslog()
{
	return;
	//2019.11.1 不输出shmem2DB进程的日志记录
/*	
    openlog(NULL, LOG_NDELAY|LOG_PID, LOG_LOCAL0);
    syslog(4,
        "class=\"%s\" msgtype=\"%d\" subtype=\"%d\" lastm=\"%d\" severity=\"%d\" means=\"%s\" msg=\"%s\" ", 
        syslog_msg.class, syslog_msg.msgtype, syslog_msg.subtype, syslog_msg.lastm, 
        syslog_msg.severity, syslog_msg.means, syslog_msg.msg);
    closelog();
*/
}

int log_merge_sql_init()
{
    int ret = 0;
    
    unsigned int i = 0;

    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    //db_res_printf(row, col, data_res);

    char log_merge_name[LOG_NAME_LEN] = {0};

    char query_sql_buf[SQL_SCRIPT_MAX_LEN] = {0};
    char merge_sql[SQL_SCRIPT_MAX_LEN] = {0};

    char msg_syslog_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};

    for(i=0; i<LOG_TYPE_NUM; i++)
    {
        snprintf(query_sql_buf, sizeof(query_sql_buf), "show create table %s;", ngfw_log[i].log_prefix);

        ret = log_backup_sql_query(query_sql_buf, &row, &col, data_res);
        if(ret != 0)
        {
            syslog_msg.severity = 752;
            snprintf(syslog_msg.msg, sizeof(syslog_msg.msg), "数据库表 [%s] 错误!", ngfw_log[i].log_cnname);
            strncpy(syslog_msg.means, "重新创建数据库表", sizeof(syslog_msg.means));
            log_backup_syslog();
            return -1;
        }
        
        //略过不需要统一merge的表，目前仅检查表是否存在
        if(ngfw_log[i].enable_merge != 1)
        {
            continue;
        }

        //db_res_printf(row, col, data_res);

        snprintf(merge_sql, sizeof(merge_sql), "%s", data_res[1][1]);

        //消除返回语句中AUTO_INCREMENT=字段对后续建表的影响,=很关键！
        char * pauto = strstr(merge_sql,"AUTO_INCREMENT=");
        if(pauto != NULL)
            *pauto = '\0';

        strcpy(ngfw_log[i].log_base, merge_sql);

        snprintf(log_merge_name, sizeof(log_merge_name), "%s_merge", ngfw_log[i].log_prefix);
        str_replace(merge_sql, ngfw_log[i].log_prefix, log_merge_name);//替换表名为xx_merge

        str_replace(merge_sql, "MyISAM", "MRG_MyISAM");//替换存储引擎为MRG_MyISAM
        printf("merge_sql = %s\n", merge_sql);

        strcpy(ngfw_log[i].log_merge_base, merge_sql);
    }

    //shmem2DB启动时，创建所有类型日志的merge表
    update_merge(LOGTYPE_MAX);

    return 0;
}

//删除指定某一天的所有日志，根据日志字符串来指定日期
static int log_backup_del_one_day_log(const char *day_str)//20190714
{
    int ret;

    long i,j;
    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    const char * get_day_table_t = "SELECT table_name FROM information_schema.tables WHERE table_schema='"DB_NAME"' "
        "AND table_type='base table' AND table_name like '%_%s%';";

    char get_day_table[SQL_SCRIPT_MAX_LEN] = {0};
    snprintf(get_day_table, sizeof(get_day_table), get_day_table_t, day_str);

    //printf("get_day_table = %s\n", get_day_table);

    ret = log_backup_sql_query(get_day_table, &row, &col, data_res);
    if(ret != 0) return -1;
    //db_res_printf(row, col, data_res);

    for(i=1; i<row+1&&i<SQL_MAX_ROW_LEN; i++)//一天最多所有分表应该达不到1024个
    {
        log_backup_del_table(data_res[i][0]);
        //printf("drop table %s;\n", data_res[i][0]);
    }
}

//返回机器上最早日志的日期，格式如20190714
//注意其他表，如logspec、xx_merge等对查询结果的影响
static time_t log_backup_get_earliest_day(char *day_str)//20190714
{
    int ret;

    long i,j;
    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    //删除超期的日志,注意merge表会影响查询结果
    const char * query_str = "SELECT table_name,create_time FROM information_schema.tables "
        "WHERE table_schema='"DB_NAME"' AND table_type='base table' "
        "AND ENGINE!='MRG_MYISAM' order by create_time asc;";
    ret = log_backup_sql_query(query_str, &row, &col, data_res);
    if(ret != 0) return -1;
    //db_res_printf(row, col, data_res);

    char *pdate = NULL;
    char date_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};

    struct tm tm_time;
    time_t time_table_create = 0;


    if(row<1)
    {
        printf("error---%s\n",__FUNCTION__);
        return -1;
    }

    for(i=1; i<row+1 && i<SQL_MAX_ROW_LEN; i++)
    {
        if((pdate = strrchr(data_res[i][0],'_'))==NULL)//没有下划线命名的日志表
            continue;

        if(pdate&&strlen(pdate)!=13)//有下划线，但不是以日期结尾，201907160000
            continue;

        strptime(data_res[i][1], "%Y-%m-%d %H:%M:%S", &tm_time);
        time_table_create = mktime(&tm_time);

        memset(date_buf,0,sizeof(date_buf));
        strncpy(date_buf,pdate+1,8);//such as '20190715'

        strncpy(day_str, date_buf, sizeof(date_buf));
        
        break;
    }

    return time_table_create;
}


//删除最早一天的日志
time_t log_backup_del_earliest_day()
{
    time_t time_table_create;
    char date_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};
    
    time_table_create = log_backup_get_earliest_day(date_buf);
    if(time_table_create < 0)
    {
        //printf("get log the earliest day error\n");
        return -1;
    }

    //printf("the earliest day is: %s\n",date_buf);

    //对于初装的设备，没有日志分表，返回的date_buf为空
    if(strlen(date_buf)<8)
        return 0;

    stop_logging();
    //删除该天的所有日志表
    log_backup_del_one_day_log(date_buf);

    //重新生成merge表
    update_merge(LOGTYPE_MAX);//所有类型

    start_logging();
    
    return time_table_create;
}

//删除超期日志，每次调用只会删除最多一天的日志
//一般来说，超期的日志不会多于一天，因此不用循环删除，而且程序会以20秒周期进行检查，无需进行循环删除
int log_backup_del_time_exceed()
{
    int ret;

    time_t time_table_create;
    time_t time_now = 0;

    //获取最早时间的日志表的生成日期
    char date_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};
    
    time_table_create = log_backup_get_earliest_day(date_buf);
    if(time_table_create < 0) return -1;

    //对于初装的设备，没有日志分表，返回的date_buf为空,返回time_table_create=0
    if(time_table_create == 0) 
    {
        //printf("no time exceed log table found!\n");
        return 0;
    }
    //printf("the earliest day is: %s\n",date_buf);

    time(&time_now);
    //printf("time_now=%d, time_table_create=%d\n",time_now,time_table_create);

    //需要添加abs，避免修改设备时间导致的数据库表混乱
    if(abs(time_now - time_table_create) > log_backup_conf.sql_table_retain_time_max)
    {
        stop_logging();
        
        //删除超期一天的所有日志
        log_backup_del_one_day_log(date_buf);

        //重新生成merge表，所有类型
        update_merge(LOGTYPE_MAX);

        start_logging();
    }
    //else
        //printf("no log time exceed!\n");

    return 0;
}

static int log_backup_get_log_base_date(const char * log_prefix, char *day_str)//201907140000
{
    int ret;

    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    //获取基础表ipf_log的创建日期
    char get_base_table_t[SQL_SCRIPT_MAX_LEN] = "select date_format((SELECT create_time FROM information_schema.tables WHERE table_schema='"DB_NAME"' AND table_name='";

    char get_base_table[SQL_SCRIPT_MAX_LEN] = {0};
    strcpy(get_base_table, get_base_table_t);
    strcat(get_base_table, log_prefix);
    strcat(get_base_table, "'),\"%Y%m%d%H%i\");");

    //printf("get_base_table %s\n", get_base_table);

    ret = log_backup_sql_query(get_base_table, &row, &col, data_res);
    if(ret != 0) return -1;
    //db_res_printf(row, col, data_res);

    if(row==1)
    {
        strncpy(day_str, data_res[1][0], strlen(data_res[1][0]));

        //printf("day_str: %s  data_res[1][0]: %s\n", day_str, data_res[1][0]);
        return 0;
    }
    else
    {
        printf("error---%s\n",__FUNCTION__);
        return -1;
    }
}

//获取目前设备上的日志表总数
int log_backup_get_log_num()
{
    int ret;

    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    //获取基础表ipf_log的创建日期
    char get_table_num_t[SQL_SCRIPT_MAX_LEN] = "SELECT count(*) FROM information_schema.tables WHERE table_schema='WFGN' AND table_type='base table';";

    ret = log_backup_sql_query(get_table_num_t, &row, &col, data_res);
    if(ret != 0) return -1;
    //db_res_printf(row, col, data_res);

    if(row==1)
    {
        //printf("get_table_num_total = %d\n", atoi(data_res[1][0]));
        return atoi(data_res[1][0]);
    }
    else
    {
        printf("error---%s\n",__FUNCTION__);
        return -1;
    }
}

//获取指定日志表的行数Row
static int log_backup_get_table_row_num(const char * table_name)
{
    int ret = 0;

    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];
    
    //获取指定表的创建日期
    char sql_get_table_row_num_t[SQL_SCRIPT_MAX_LEN] = "SELECT count(*) FROM %s;";
    char sql_get_table_row_num[SQL_SCRIPT_MAX_LEN] = {0};

    snprintf(sql_get_table_row_num,sizeof(sql_get_table_row_num),sql_get_table_row_num_t,table_name);
    ret = log_backup_sql_query(sql_get_table_row_num,&row, &col, data_res);
    if((ret != 0) || (row != 1) || (col != 1) || (!data_res[1][0]))
    {
        printf("error---%s, table= %s\n",__FUNCTION__, table_name);
        return -1;
    }

    return atoi(data_res[1][0]);
}


//获取查询日期的日志表数量
static int log_backup_get_day_log_num(const char *day_str)//20190714
{
    int ret = 0;

    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    //获取基础表ipf_log的创建日期
    char get_table_num_t[SQL_SCRIPT_MAX_LEN] = "SELECT count(*) FROM information_schema.tables WHERE table_schema='WFGN' AND table_type='base table' AND table_name like '%_";

    char get_table_num[SQL_SCRIPT_MAX_LEN] = {0};
    strcpy(get_table_num, get_table_num_t);
    strcat(get_table_num, day_str);
    strcat(get_table_num, "%';");

    //printf("get_table_num %s\n", get_table_num);

    ret = log_backup_sql_query(get_table_num, &row, &col, data_res);
    if(ret != 0) return -1;
    //db_res_printf(row, col, data_res);

    if(row==1)
    {
        //printf("get_table_num %d\n", atoi(data_res[1][0]));
        return atoi(data_res[1][0]);
    }
    else
    {
        printf("error---%s\n",__FUNCTION__);
        return -1;
    }
}

//返回基础表的创建日期，必须指定日志类型
static time_t log_backup_get_log_base_time(log_type logtype)
{
    int ret=0;

    long i=0,j=0;
    long row = 0;
    long col = 0;
    char *data_res[SQL_MAX_ROW_LEN][SQL_MAX_COLUMN_LEN];

    struct tm tm_time;
    time_t time_table_create;

    //获取base表的创建日期
    char query_str_t[SQL_SCRIPT_MAX_LEN] = "SELECT table_name,create_time FROM information_schema.tables "
        "WHERE table_schema='"DB_NAME"' AND table_type='base table' "
        "AND table_name='%s';";
    char query_str[SQL_SCRIPT_MAX_LEN];

    int i_type = 0;
    while( i_type < LOG_TYPE_NUM)
    {
        //略过不需要统一merge的表，此分支应该不会触发
        if(ngfw_log[i_type].enable_merge != 1)
        {
            i_type++;
            continue;
        }
        
        if(ngfw_log[i_type].logtype == logtype)
        {
            snprintf(query_str,sizeof(query_str), query_str_t, ngfw_log[i_type].log_prefix);
            ret = log_backup_sql_query(query_str, &row, &col, data_res);
            if(ret != 0) return -1;
            //db_res_printf(row, col, data_res);

            strptime(data_res[1][1], "%Y-%m-%d %H:%M:%S", &tm_time);
            time_table_create = mktime(&tm_time);

            return time_table_create;
        }
        i_type++;
    }

    return -1;
}

//重命名当前的日志base表，创建新的base表
//LOGTYPE_MAX  --  为所有类型
//其他  --  指定一种类型
static int log_backup_base_table_backup(log_type logtype)
{
    int ret = 0;

    char date_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};
    char sql_script_buf[SQL_SCRIPT_MAX_LEN] = {0};
    char sql_table_name[LOG_NAME_LEN] = {0};

    int i_type = 0;
    while( i_type < LOG_TYPE_NUM)
    {
        //略过不需要统一merge的表
        if(ngfw_log[i_type].enable_merge != 1)
        {
            i_type++;
            continue;
        }
        
        if(ngfw_log[i_type].logtype == logtype || logtype == LOGTYPE_MAX)
        {
            //获取base表的创建日期--作为重命名的文件名
            memset(date_buf,0,sizeof(date_buf));
            ret = log_backup_get_log_base_date(ngfw_log[i_type].log_prefix, date_buf);
            if(ret < 0) return -1;

            //重命名旧base表
            snprintf(sql_table_name,sizeof(sql_table_name), "%s_%s", ngfw_log[i_type].log_prefix, date_buf);
            snprintf(sql_script_buf,sizeof(sql_script_buf), "rename table %s to %s;", ngfw_log[i_type].log_prefix, sql_table_name);

            //printf("sql_script_buf %s\n", sql_script_buf);
            //printf("ngfw_log[i_type].log_base %s", ngfw_log[i_type].log_base);

            if(mysql_query(mysql,sql_script_buf))
            {
                //可能出现 Table 'xxx' already exists 的错误
                //printf("mysql_query error: %s\n", mysql_error(mysql));

                //表已存在（error code - 1050），则暴力删除之前的同名表
                if(mysql_errno(mysql) == 1050)
                {
                    log_backup_del_table(sql_table_name);
                    //再次尝试重命名旧base表
                    if(mysql_query(mysql,sql_script_buf))
                        return -1;
                }
                else
                {
                    return -1;
                }
            }

            //创建新base表
            if(mysql_query(mysql, ngfw_log[i_type].log_base))
            {
                printf("mysql_query error: %s\n", mysql_error(mysql));
                return -1;
            }

        }
        i_type++;
    }

    return 0;
}

//备份前一天所有日志，根据当前“ipf_log”的时间，备份其上一天的日志表
//根据目前的设计，不能获取当天的备份日志包
static int log_backup_backup_day_log()
{
    int ret = 0;

    char date_buf[LOG_BACKUP_MAX_STRING_LEN] = {0};
    char data_backup[LOG_BACKUP_MAX_STRING_LEN] = {0};
    char log_data_backup[LOG_BACKUP_MAX_STRING_LEN] = {0};
    
    ret = log_backup_get_log_base_date(ngfw_log[0].log_prefix, date_buf);//以ipf_log为时间基准
    if(ret < 0) return -1;

    memset(data_backup,0,sizeof(data_backup));
    strncpy(data_backup,date_buf,8);//such as '20171226'

    int log_day = atoi(data_backup) - 1;//前一天日期字符串
    snprintf(log_data_backup, sizeof(log_data_backup), "%d", log_day);

    //根据日期字符串，获取备份当天的日志表数量，肯定不会包括base表
    int table_num = log_backup_get_day_log_num(log_data_backup);

    if(table_num == 0)
    {
        //printf("%s has no log backup table", log_data_backup);
        return 0;
    }
    else if(table_num < 0)
    {
        //printf("log backup table has error!");
        return -1;
    }
    
    char shell_scritp_buf[SQL_SCRIPT_MAX_LEN] = {0};
    snprintf(shell_scritp_buf,sizeof(shell_scritp_buf),
            "mkdir -p /var/lib/mysql/backup && cd /var/lib/mysql/WFGN/ && "
            "tar cf ../backup/log_%s.zip *%s* & > /dev/null "
            "&&  cd - > /dev/null;",log_data_backup, log_data_backup);
    printf("backup_script : %s", shell_scritp_buf);

    system(shell_scritp_buf);

    return 0;
}

int log_backup_backup(int interval)
{
    unsigned int i;

    time_t time_now = 0;
    time(&time_now);

    //printf("time_now=%d\n",time_now);

    int minites = time_now/60;
    minites +=(8*60);//time zone adjust
    int interval_min = interval/60;

    static int backup_flag = 0;

    //零点备份检查
    if((minites%interval_min == 0)&&(backup_flag == 0))
    {
        printf("start day log backup!\n");
        stop_logging();

        log_backup_base_table_backup(LOGTYPE_MAX);
        update_merge(LOGTYPE_MAX);

        //备份前一天所有日志
        log_backup_backup_day_log();

        start_logging();
        
        backup_flag = 1;
    }
    else if((minites%interval_min)&&(backup_flag == 1))
        backup_flag = 0;

    for(i=0; i<LOG_TYPE_NUM; i++)
    {
        //略过不需要统一merge的表
        if(ngfw_log[i].enable_merge != 1)
        {
            continue;
        }
        
        //单表时间超限
        time_t time_create_table = log_backup_get_log_base_time(ngfw_log[i].logtype);
        if(time_create_table < 0) return -1;

        time_now = time(&time_now);
        
        //需要添加abs，避免修改设备时间导致的数据库表混乱
        if(abs(time_now - time_create_table) > LOG_BACKUP_TABLE_BACKUP_INTERVAL)
        {
            //printf("Table [ %s ] sql_table_backup_interval exceed!\n", ngfw_log[i].log_prefix);
            //printf("sql_table_backup_interval -- stop_logging\n");
            stop_logging();
            
            log_backup_base_table_backup(ngfw_log[i].logtype);
            update_merge(ngfw_log[i].logtype);

            start_logging();
            //printf("sql_table_backup_interval -- start_logging\n");
        }

        //单表条数超限
        int base_table_num = log_backup_get_table_row_num(ngfw_log[i].log_prefix);
        if(base_table_num > log_backup_conf.sql_table_row_max)
        {
            //printf("Table [ %s ] sql_table_row_max exceed!\n", ngfw_log[i].log_prefix);
            //printf("sql_table_row_max -- stop_logging\n");
            stop_logging();
            
            log_backup_base_table_backup(ngfw_log[i].logtype);
            update_merge(ngfw_log[i].logtype);
            
            start_logging();
            //printf("sql_table_row_max -- start_logging\n");
        }
    }
}

//删除指定日期的日志备份包
static int log_backup_delete_table_backup_file(const char *day_str)//20190716
{
    char log_file_path[LOG_BACKUP_MAX_STRING_LEN] = {0};

    if(!day_str)
        return -1;

    snprintf(log_file_path,sizeof(log_file_path),
        "rm -f /var/lib/mysql/backup/log_%s.zip;",day_str);
    
    system(log_file_path);
    
    return 0;
}

/*
https://blog.csdn.net/qq_32261101/article/details/87563410
*/
static int str_replace(char strRes[],const char from[], const char to[])
{
    int flag=0;
    char *Ptr = NULL;
    char *middle_flag = strstr(strRes, from);
    if(middle_flag == NULL)
    {
        return flag;
    }

    int len = strlen(middle_flag);
    Ptr = (char *)malloc(len * sizeof(char));
    if (NULL == Ptr)
    {
        return flag;
    }
    strcpy(Ptr, middle_flag + (strlen(from)));
    if (middle_flag != NULL)
    {
        /* code */
        *middle_flag = '\0';
        strcat(strRes, to);
        strcat(strRes, Ptr);
        free(Ptr);
        flag = 1;
    }
    return flag;
}

static int get_loggather_pid()
{
    FILE *fp;
    char buf[LOG_BACKUP_MAX_STRING_LEN];
    int pid;

    fp = fopen(LOG_BACKUP_LOGGATHER_PID_FILE, "r");//打开文件
    if (NULL != fp)
    {
        if( fgets(buf, LOG_BACKUP_MAX_STRING_LEN-1, fp)== NULL )
        {
            fclose(fp);
            printf("can not get PID!");
            return -1;
        }
        //printf("PID:  %s\n", buf);
        fclose(fp);

        return atoi(buf);
    }
    else
    {
        //printf("open PID file error!\n");
        return -1;
    }

}

/**
 *  * @brief block to byte
 *  * @param b     block
 *  * @param bs     KByte
 *  * @return         Kbyte
 *      
 */
static unsigned long kscale(unsigned long b, unsigned long bs)
{
    return (b * (unsigned long long) bs + 1024/2) / 1024;
}

/**
 *  * @name convert_size
 *  * @brief convert size to GB MB KB
 *  *         size:    the size
 *  *         dest:    the return size like string
 *  * @author JerryLeo
 *  * @date 2009-12-12
 *  
 */
static char *convert_size(float size, char *dest)
{
    if((((size/1024.0)/1024.0)) >= 1.0){
        sprintf(dest, "%0.2fGB", ((size/1024.0)/1024.0));
    }
    else if(((size/1024.0)) >= 1.0){
        sprintf(dest, "%0.2fMB", (size/1024.0));
    }
    else{
        sprintf(dest, "%0.2fKB", size);
    }
    return dest;
}

/**
 *  * @brief get disk informations
 *  * @param path     the disk mount point
 *  * @param di     the structure of the disk information
 *  * @return         success: 1, faild: 0
 *  
 */
static int get_disk_information(const char *path, struct disk_infomation *di)
{
    struct statfs buf;
    int i=0;
    float disk_total_size = 0;
    float disk_free_size = 0;
    float disk_usage_size = 0;

    i=statfs(path, &buf);
    if(i < 0){
        printf("get disk infomation faild\n");
        return 0;
    }
    memset(di->disk_format, 0, sizeof(di->disk_format));

    switch(buf.f_type)
    {
        case 0x4d44:
        sprintf(di->disk_format, "FAT");
        break;

        case 0x5346544e:
        case 0X65735546:
        sprintf(di->disk_format, "NTFS");
        break;

        case 0xEF53:
        case 0xEF51:
        sprintf(di->disk_format, "EXT*");
        break;

        case 0xadf5:
        sprintf(di->disk_format, "ADFS_SUPER_MAGIC");
        break;

        case 0xADFF:
        sprintf(di->disk_format, "AFFS_SUPER_MAGIC");
        break;

        case 0x42465331:
        sprintf(di->disk_format, "BEFS_SUPER_MAGIC");
        break; 

        case 0x1BADFACE:
        sprintf(di->disk_format, "BFS_MAGIC");
        break;
              
        case 0xFF534D42:
        sprintf(di->disk_format, "CIFS_MAGIC_NUMBER");
        break;

        case 0x73757245:
        sprintf(di->disk_format, "CODA_SUPER_MAGIC");
        break;
             
        case 0x012FF7B7:
        sprintf(di->disk_format, "COH_SUPER_MAGIC");
        break;

        case 0x28cd3d45:
        sprintf(di->disk_format, "CRAMFS_MAGIC");
        break;

        case 0x1373:
        sprintf(di->disk_format, "DEVFS_SUPER_MAGIC");
        break;
            

        case 0x00414A53:
        sprintf(di->disk_format, "EFS_SUPER_MAGIC");
        break;	

        case 0x137D:
        sprintf(di->disk_format, "EXT_SUPER_MAGIC");
        break;
               
        case 0x4244:
        sprintf(di->disk_format, "HFS_SUPER_MAGIC");
        break;     

        case 0xF995E849:
        sprintf(di->disk_format, "HPFS_SUPER_MAGIC");
        break; 

        case 0x958458f6:
        sprintf(di->disk_format, "HUGETLBFS_MAGIC");
        break; 
              
        case 0x9660:
        sprintf(di->disk_format, "ISOFS_SUPER_MAGIC");
        break; 

        case 0x72b6:
        sprintf(di->disk_format, "JFFS2_SUPER_MAGIC");
        break;

        case 0x3153464a:
        sprintf(di->disk_format, "JFS_SUPER_MAGIC");
        break; 

        case 0x137F:
        sprintf(di->disk_format, "MINIX_SUPER_MAGIC");
        break; 

        case 0x138F:
        sprintf(di->disk_format, "MINIX_SUPER_MAGIC2");
        break;

        case 0x2468:
        sprintf(di->disk_format, "MINIX2_SUPER_MAGIC");
        break;

        case 0x2478:
        sprintf(di->disk_format, "MINIX2_SUPER_MAGIC2");
        break;

        case 0x564c:
        sprintf(di->disk_format, "NCP_SUPER_MAGIC");
        break;

        case 0x6969:
        sprintf(di->disk_format, "NFS_SUPER_MAGIC");
        break;

        case 0x9fa1:
        sprintf(di->disk_format, "OPENPROM_SUPER_MAGIC");
        break;

        case 0x9fa0:
        sprintf(di->disk_format, "PROC_SUPER_MAGIC");
        break;

        case 0x002f:
        sprintf(di->disk_format, "QNX4_SUPER_MAGIC");
        break;

        case 0x52654973:
        sprintf(di->disk_format, "REISERFS_SUPER_MAGIC");
        break;

        case 0x7275:
        sprintf(di->disk_format, "ROMFS_MAGIC");
        break;	    

        case 0x517B:
        sprintf(di->disk_format, "SMB_SUPER_MAGIC");
        break;      

        case 0x012FF7B6:
        sprintf(di->disk_format, "SYSV2_SUPER_MAGIC");
        break;  

        case 0x012FF7B5:
        sprintf(di->disk_format, "SYSV4_SUPER_MAGIC");
        break;   

        case 0x01021994:
        sprintf(di->disk_format, "TMPFS_MAGIC");
        break;     

        case 0x15013346:
        sprintf(di->disk_format, "UDF_SUPER_MAGIC");
        break;      

        case 0x00011954:
        sprintf(di->disk_format, "UFS_MAGIC");
        break;     

        case 0x9fa2:
        sprintf(di->disk_format, "USBDEVICE_SUPER_MAGIC");
        break;           

        case 0xa501FCF5:
        sprintf(di->disk_format, "VXFS_SUPER_MAGIC");
        break;       

        case 0x012FF7B4:
        sprintf(di->disk_format, "XENIX_SUPER_MAGIC");
        break;    

        case 0x58465342:
        sprintf(di->disk_format, "XFS_SUPER_MAGIC");
        break;   

        case 0x012FD16D:
        sprintf(di->disk_format, "_XIAFS_SUPER_MAGIC");
        break;            
               

        default:
        printf("tyep=%x\n",buf.f_type);
        sprintf(di->disk_format, "unknown");
        break;
    }


    memset(di->disk_total_capacity, 0, sizeof(di->disk_total_capacity));
    memset(di->disk_free_capacity, 0, sizeof(di->disk_free_capacity));
    memset(di->disk_usage_capacity, 0, sizeof(di->disk_usage_capacity));

    disk_total_size =
            (float)(kscale(buf.f_blocks, buf.f_bsize));
    disk_free_size =
            (float)(kscale(buf.f_bfree, buf.f_bsize));
    disk_usage_size = disk_total_size - disk_free_size;
    sprintf(di->disk_usage_capacity,"%0.2f",(float)(disk_usage_size/disk_total_size)*100);

    convert_size(disk_total_size, di->disk_total_capacity);
    convert_size(disk_free_size, di->disk_free_capacity);

    memset(di->disk_permissions, 0, sizeof(di->disk_permissions));
    sprintf(di->disk_permissions, "rw\n");
    return 1;
}

/** 
 * function: check the file system capacity
@param path : the file system path mounted
@param percent  : percent which will check aimed
@return:
    0   : checked , >percent
    !0  : error code , <percent
*/
int disk_capacity_check(const char * path,int percent)
{
    float disk_usage = 0.0;
    if(!path)
        return -1;
    if(percent < 0 || percent > 100)
        return -1;
    struct disk_infomation *di=malloc(sizeof(struct disk_infomation));


    get_disk_information(path, di);

    disk_usage = atof(di->disk_usage_capacity);

    if(disk_usage > percent)
    {
        if(di)
            free(di);
        return 0;
    }

    if(di)
        free(di);
    
    return -1;
}

/** 
 * function: printf file system capacity information
@param path :  the file system path mounted
@return:
    0   : success
    !0  : error code
*/
int disk_capacity_print(const char * path)
{
    if(!path)
        return -1;
    struct disk_infomation *di=malloc(sizeof(struct disk_infomation));
    

    get_disk_information(path, di);

    printf("DiskType:            %s\n", di->disk_format);
    printf("DiskTotalCapacity    %s\n", di->disk_total_capacity);
    printf("DiskFreeCapacity    %s\n", di->disk_free_capacity);
    printf("DiskUsageCapacity     %s\%\n", di->disk_usage_capacity);
    printf("DiskPermissions        %s\n", di->disk_permissions);

    if(di)
        free(di);
    return 0;
}


