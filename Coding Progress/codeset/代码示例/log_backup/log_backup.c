/*******************************************************************************
*	* @file log_backup.c
*   * @brief log backup thread
*	* @author 
*	* @date 20190716  
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

//全局变量
DATABASE_CONN log_backup_dbconn =  {
        NULL,           //MYSQL*
        "localhost",    //host
        "root",         //user
        "",             //pwd
        DB_NAME,        //DB name
        0               //port
};

struct log_backup_conf_t log_backup_conf={
    DISK_FILE_SYSTEM_PATH,
    DISK_CAPACITY_WARN_PERCENT,
    DISK_CAPACITY_CHECK_PERCENT,
    DISK_CAPACITY_UPPER_LIMIT_ACTION_STOP,
    LOG_BACKUP_MAX_TABLE_RETAIN_TIME,
    LOG_BACKUP_TABLE_BACKUP_INTERVAL,//零点备份
    LOG_BACKUP_TABLE_ROW_MAX
};

log_syslog_msg syslog_msg=
{
    "DEV",
    451,//系统消息
    410,//系统消息
    1,
    750,//750-一般，751-中等，752-严重，753-通知
    "",
    ""
};


//全局变量定义
SHMEM2DB_THREAD_INFO thread_for_log_backup;

//全局日志记录启停标识
int is_stop_log = 0;

//全局mysql
MYSQL *mysql = NULL;


//静态函数声明
static void log_backup_thread_func(void);

/** 
 * function: init the log backup module 
@param null
@return:
	0	: success
	!0	: error code
*/
int log_backup_module_init(void)
{
    if(initDBconnInfo(&log_backup_dbconn, SHMEM_DBCON_INI_FILE) != 0)
    {
        perror("Failed to init DBconn Info by inifile,use default DBconfig.\n");
        return -1;
    }
    if(connectDatabase(&log_backup_dbconn))
    {
        perror("Failed to login database.\n");
        return -1;
    }
    if(log_backup_config_load(LOG_BACKUP_CONFIG_INI_FILE))
    {
        perror("Failed to load config.\n");
        return -1;
    }

    mysql = log_backup_dbconn.mysql;

    if(log_merge_sql_init())
    {
        syslog_msg.severity = 752;//严重
        strncpy(syslog_msg.msg, "日志记录进程启动失败", sizeof(syslog_msg.msg));
        strncpy(syslog_msg.means, "重新创建数据库表", sizeof(syslog_msg.means));
        log_backup_syslog();

        perror("Failed to log_merge_sql_init.\n");
        return -1;
    }

    syslog_msg.severity = 753;//通知
    strncpy(syslog_msg.msg, "日志记录进程启动成功", sizeof(syslog_msg.msg));
    strncpy(syslog_msg.means, "", sizeof(syslog_msg.means));
    log_backup_syslog();
    
    /* start log_backup thread */
    initThread(&thread_for_log_backup, (void *)log_backup_thread_func,log_backup_dbconn);
    pthread_detach(thread_for_log_backup.thread);	

    return 0;
}

/** 
 * function: free the log backup module resource
@param void
@return void
	
*/
void log_backup_free_resc(void)
{
    printf("Releasing log backup resources...\n");

    // disconnect db 
    if(log_backup_dbconn.mysql)
    {
        mysql_close(log_backup_dbconn.mysql);
        log_backup_dbconn.mysql = NULL;
    }

    //release thread 
    if(threadClean(thread_for_log_backup) == -1)
        printf("Failed to release thread_for log_backup resources.\n");
    else
        printf("Thread for log_backup cleaned.\n");
}

/*
 ****************************************************************************
 *                     LOCAL FUNCTIONS
 ****************************************************************************
 */

 /** 
  * function: main thread process  
 @param void
 @return void
 */
 static void log_backup_thread_func(void)
 {
     sigset_t signal_set;    

     sigfillset (&signal_set);//将参数set信号集初始化，然后把所有的信号加入到此信号集里即将所有的信号标志位置，屏蔽所有的信号
     pthread_sigmask (SIG_BLOCK, &signal_set, NULL );
     if (pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL))//异步取消，线程接收到取消操作后，立即取消
     {
         printf("Failed to set thread cancel type to PTHREAD_CANCEL_ASYNCHRONOUS.\n");
     }
 
     while(1)
     {
         //disk_capacity_print(log_backup_conf.disk_file_system_path);
         
         int ret_cap_chk = disk_capacity_check(log_backup_conf.disk_file_system_path, log_backup_conf.disk_capacity_check_percent);
 
         //超过容量百分比 或者 设备上日志表总数过大
         if(ret_cap_chk == 0 || (log_backup_get_log_num()>SQL_MAX_ROW_LEN_LIMIT))
         {
             if(log_backup_conf.disk_capacity_limit_action == DISK_CAPACITY_UPPER_LIMIT_ACTION_REMOVE)
                 (void)log_backup_del_earliest_day();
             else if(log_backup_conf.disk_capacity_limit_action == DISK_CAPACITY_UPPER_LIMIT_ACTION_STOP)
                 stop_logging();
         }
         else
             start_logging();
 
         //删除超期日志，超过预设天数
         log_backup_del_time_exceed();
 
         //日志备份主要工作
         log_backup_backup(log_backup_conf.sql_table_backup_interval);

         //printf("is_stop_log = %d\n", is_stop_log);
         
         sleep(LOG_BACKUP_THREAD_CHECK_INTERVAL);
     }
 }
 
