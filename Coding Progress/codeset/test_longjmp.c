#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
sigjmp_buf jmp_env;

int xx = 1;

static void connect_alarm()
{
    xx = 1;
    siglongjmp(jmp_env, 1);
}

int main()
{
    // 当超时时间sec_timeout大于等于运行时间run_time时会跳过printf("running...\n");
    int sec_timeout = 3;
    int run_time = 4;

    int xxx = 100;
    int xxxx = 11;
  //  xx = 1;
    printf("xx = %d\n", xx);
    printf("timeout = %d, run time = %d\n", sec_timeout, run_time);
    if (sec_timeout)
    {
        // 超过用alarm函数设置的时间时产生此信号，调用connect_alarm函数
        signal(SIGALRM, connect_alarm);
        alarm(sec_timeout);
        printf("set timeout\n");
//    	xx = 0;
	printf("before jmp, xx = %d\n", xx);
        if (sigsetjmp(jmp_env, 1))
        {
            printf("timeout\n");
            printf("xxx = %d\n", xxx);
            goto out;
        }
	xx = 0;
	xxx = 0;
	printf("after jmp, xx = %d\n", xx);
    }

    sleep(run_time);
    printf("xx = %d\n", xx);
    printf("running...\n");

out:
    if (sec_timeout)
    {
        // 取消先前设置的闹钟
        alarm(0);
    	printf("xx = %d\n", xx);
    	printf("xxx = %d\n", xxx);
        printf("cancel timeout\n");
    }

    return 0;
}
