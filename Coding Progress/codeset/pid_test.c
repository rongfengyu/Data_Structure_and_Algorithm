#include <stdio.h>
#include <inttypes.h>   // PRIuMAX                                                  
#include <errno.h>      // errno                                                    
#include <string.h>     // strerror                                                 
                                                                                    
#include <sys/types.h>                                                              
#include <sys/stat.h>                                                               
#include <fcntl.h>                                                                  
#include <unistd.h> // getpid                                                       
#include <signal.h> // kill                                                         
                                                                                    
#include <stdio.h>                                  
#include <unistd.h>                                                                             
                                                                                                                                                     
#ifndef LOG_PRINT                                                                                                     
#define LOG_MAX_LOG_MSG_LEN 2048                                                                                      
#define Log(x, file, func, line, ...)                           \
    do {                                                        \
        char _log_msg[LOG_MAX_LOG_MSG_LEN];                     \
        snprintf(_log_msg, LOG_MAX_LOG_MSG_LEN, __VA_ARGS__);   \         
        fprintf(stdout, "<%s> %s\n", x, _log_msg);              \
    } while(0)                                                            
                                                                          
#define LogError(...) Log("ERROR", \                                      
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)                    
                                                                          
#define LogWarning(...) Log("WARNING", \                                  
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)                    
                                                                          
#define LogInfo(...) Log("INFO", \                                        
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)                    
                                                                          
#define LogDebug(...) Log("DEBUG", \                                      
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)                    
                                                                          
#define FatalError(...) do {                                           \
    LogError(__VA_ARGS__);                                             \
    exit(EXIT_FAILURE);                                                \
} while(0)                                                                
#endif

                                                                                 
/**                                                                                 
 * \brief Write a pid file (used at the startup)                                    
 *        This commonly needed by the init scripts                                  
 *                                                                                  
 * \param pointer to the name of the pid file to write (optarg)                     
 *                                                                                  
 * \retval 0 if succes                                                              
 * \retval -1 on failure                                                            
 */                                                                                 
int PidfileCreate(const char *pidfile)                                              
{                                                                                   
    int pidfd = 0;                                                                  
    char val[16];                                                                   
                                                                                    
    int len = snprintf(val, sizeof(val), "%"PRIuMAX"\n", (uintmax_t)getpid());      
    if (len <= 0) {                                                                 
        LogError("Pid error (%s)", strerror(errno));                                
        return(-1);                                                                 
    }                                                                               
                                                                                    
    pidfd = open(pidfile, O_CREAT | O_TRUNC | O_NOFOLLOW | O_WRONLY, 0644);         
    if (pidfd < 0) {                                                                
        LogError("unable to set pidfile '%s': %s",                                  
                   pidfile,                                                         
                   strerror(errno));                                                
        return(-1);                                                                 
    }                                                                               
                                                                                    
    ssize_t r = write(pidfd, val, (unsigned int)len);                               
    if (r == -1) {                                                                  
        LogError("unable to write pidfile: %s", strerror(errno));                   
        close(pidfd);                                                               
        return(-1);                                                                 
    } else if ((size_t)r != len) {                                                  
        LogError("unable to write pidfile: wrote"                                   
                " %"PRIdMAX" of %"PRIuMAX" bytes.", (intmax_t)r, (uintmax_t)len);   
        close(pidfd);                                                               
        return(-1);                                                                 
    }                                                                               
                                                                                    
    close(pidfd);                                                                   
    return(0);                                                                      
}                                                                                   
                                                                                    
/**                                                                                 
 * \brief Remove the pid file (used at the startup)                                 
 *                                                                                  
 * \param pointer to the name of the pid file to write (optarg)                     
 */                                                                                 
void PidfileRemove(const char *pid_filename)                                        
{                                                                                   
    if (pid_filename != NULL) {                                                     
        /* we ignore the result, the user may have removed the file already. */     
        (void)unlink(pid_filename);                                                 
    }                                                                               
}                                                                                   
                                                                                    
/**                                                                                 
 * \brief Check a pid file (used at the startup)                                    
 *        This commonly needed by the init scripts                                  
 *                                                                                  
 * \param pointer to the name of the pid file to write (optarg)                     
 *                                                                                  
 * \retval 0 if succes                                                              
 * \retval -1 on failure                                                            
 */                                                                                 
int PidfileTestRunning(const char *pid_filename)                                    
{                                                                                   
    if (access(pid_filename, F_OK) == 0) {                                          
        /* Check if the existing process is still alive. */                         
        pid_t pidv;                                                                 
        FILE *pf;                                                                   
                                                                                    
        pf = fopen(pid_filename, "r");                                              
        if (pf == NULL) {                                                           
            LogError("pid file '%s' exists and can not be read. Aborting!",         
                    pid_filename);                                                  
            return -1;                                                              
        }                                                                           
                                                                                    
        if (fscanf(pf, "%d", &pidv) == 1 && kill(pidv, 0) == 0) {                   
            fclose(pf);                                                             
            LogError("pid file '%s' exists. Is program already running? Aborting!", 
                    pid_filename);                                                  
            return -1;                                                              
        }                                                                           
                                                                                    
        fclose(pf);                                                                 
    }                                                                               
    return 0;                                                                       
}                                                                                                                                                                                                        
                                                    
int main()                                          
{                                                   
    int result = 0;                                 
                                                    
    const char *pidfile = "/var/run/test_pid01.pid";
                                                    
    if (PidfileTestRunning(pidfile) != 0)           
        return -1;                                  
                                                    
    PidfileCreate(pidfile);                         
                                                    
    while (1){                                      
        sleep(1);                                   
    }                                               
                                                    
    PidfileRemove(pidfile);                         
    return 0;                                       
}                                                   