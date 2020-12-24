#ifndef _LOG_BACKUP_H_
#define _LOG_BACKUP_H_

#include <time.h>
#include <stdlib.h>
#include <wfwlogger/libini.h>
#include <mysql/mysql.h>
#include "shmem2DB.h"

int  log_backup_module_init(void);
void log_backup_free_resc(void);

#endif//_LOG_BACKUP_H_

