/***********************************************************************************************************************/
/* Name: CheckWordLen                                                                                                  */
/* Function: to check system can support 32-bits and 64-bits operation whether or not                                  */
/* Author: Zhong W.J. (From ICT of CESI)                                                                               */
/* Date:2010-03                                                                                                        */
/* Version: 0.1                                                                                                        */
/* Usage: step1:gcc -o checkwordlen CheckWordLen.c; step2: ./checkwordlen                                              */
/***********************************************************************************************************************/


#include "stdio.h"

void main(int argc, char *argv[])
{
      //定义判断系统支持32位字长和64为字长的标志变量
	int Flag32,Flag64;
      //定义16位、32位以及64位整数变量
	unsigned long A16, B16, A32, B32;
	unsigned long Sum32, Sum64, Product32, Product64;

        int  ishiftvar;
        long lshiftvar;
        int  counter;

        int char_size, int_size, long_size, llong_size, charpointer_size, intpointer_size, longpointer_size;
       
       // 将变量进行复制
	Flag32=0;
	Flag64=0;
	A16=0xFFFF;
	B16=0xFFFF;
	A32=0xFFFFFFFF;
	B32=0xFFFFFFFF;

      //将两个16位整数（0xFFFF）进行加法和乘法运算，并将结果保存到一个32位整形变量中
	Sum32 = A16+B16;
	Product32 = A16*A16;

      //将两个32位整数（0xFFFFFFFF）进行加法和乘法运算，并将结果保存到一个64位整形变量中
	Sum64 = A32+B32;
	Product64 = A32*B32;
       
       /*
	if (argc > 1)
	{
		A16 = (long)atoi(argv[1]);
		B16 = (long)atoi(argv[2]);
	}
       */

        ishiftvar=0x01;
        counter=0;
        
        do 
        {
          ishiftvar = ishiftvar << 1;
          counter++;
        } while(ishiftvar !=0);
        printf(" size of ishiftvar is %d bits\n", counter);

        lshiftvar=0x01;
        counter=0;
        
        do        
        {
          lshiftvar = lshiftvar << 1;
          counter++;
        } while(lshiftvar !=0);
        printf(" size of lshiftvar is %d bits \n", counter);

        char_size        = sizeof(char);
        int_size         = sizeof(int);
        long_size        = sizeof(long);
        llong_size       = sizeof(long long);
        charpointer_size = sizeof(char *);
        intpointer_size  = sizeof(int *);
        longpointer_size  = sizeof(long long *);
        
        printf(" char_size:%d \n int_size:%d \n long_size:%d \n llong_size:%d \n charpointer_size:%d \n intpointer_size:%d \n longpointer_size:%d \n",char_size,int_size,long_size,llong_size, charpointer_size, intpointer_size, longpointer_size);

       //看16位数加、乘运算是结果保存到32位整形变量时是否溢出，从而判断系统是否支持32位数据操作
	if((Sum32 == 0x1FFFE) && (Product32 == 0xFFFE0001))
		Flag32=1;
       else
              Flag32=0;

       //看32位数加、乘运算是结果保存到32位整形变量时是否溢出，从而判断系统是否支持64位数据操作
	if((Sum64 == 0x1FFFFFFFE) && (Product64 == 0xFFFFFFFE00000001))
		Flag64=1;
       else
              Flag64=0;

       //输出结果
	switch (Flag32+Flag64)
	{
	case 1:
		printf(" This system support 32-bits Operation!!!! \n");break;
	case 2:
		printf(" This system support 64-bits Operation !!!! \n");break;
	case 0:
		printf(" This system support Operation under 32-bits!!!! \n");break;
	}
}
