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
      //�����ж�ϵͳ֧��32λ�ֳ���64Ϊ�ֳ��ı�־����
	int Flag32,Flag64;
      //����16λ��32λ�Լ�64λ��������
	unsigned long A16, B16, A32, B32;
	unsigned long Sum32, Sum64, Product32, Product64;

        int  ishiftvar;
        long lshiftvar;
        int  counter;

        int char_size, int_size, long_size, llong_size, charpointer_size, intpointer_size, longpointer_size;
       
       // ���������и���
	Flag32=0;
	Flag64=0;
	A16=0xFFFF;
	B16=0xFFFF;
	A32=0xFFFFFFFF;
	B32=0xFFFFFFFF;

      //������16λ������0xFFFF�����мӷ��ͳ˷����㣬����������浽һ��32λ���α�����
	Sum32 = A16+B16;
	Product32 = A16*A16;

      //������32λ������0xFFFFFFFF�����мӷ��ͳ˷����㣬����������浽һ��64λ���α�����
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

       //��16λ���ӡ��������ǽ�����浽32λ���α���ʱ�Ƿ�������Ӷ��ж�ϵͳ�Ƿ�֧��32λ���ݲ���
	if((Sum32 == 0x1FFFE) && (Product32 == 0xFFFE0001))
		Flag32=1;
       else
              Flag32=0;

       //��32λ���ӡ��������ǽ�����浽32λ���α���ʱ�Ƿ�������Ӷ��ж�ϵͳ�Ƿ�֧��64λ���ݲ���
	if((Sum64 == 0x1FFFFFFFE) && (Product64 == 0xFFFFFFFE00000001))
		Flag64=1;
       else
              Flag64=0;

       //������
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
