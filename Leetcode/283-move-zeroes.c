/*
283. �ƶ���
����һ������ nums����дһ������������ 0 �ƶ��������ĩβ��ͬʱ���ַ���Ԫ�ص����˳��

ʾ��:

����: [0,1,0,3,12]
���: [1,3,12,0,0]
˵��:

������ԭ�����ϲ��������ܿ�����������顣
�������ٲ���������

ע�⣺˫ָ�룬���ı����Ԫ�����˳�򣻲��ı�Ԫ�����˳������ʹ�ô�β����ʼ����Ԫ�صķ�����leetcode27��
*/

#include <stdio.h>

void moveZeroes(int nums[], int len)
{
    int tmp;
    //��ָ��������0Ԫ�ؾ�����ָ����ָ��Ԫ�ؽ���������ָ������ָ��֮���Ԫ�ض�Ϊ0
    for(int slow=0,i=0;i<len;++i)
        if(nums[i]!=0 && (slow!=i))//�Թ��Է���Ԫ�ؿ�ʼ�Ľ���
        {
            tmp=nums[i];
            nums[i]=nums[slow];
            nums[slow]=tmp;
            slow++;
        }    
            
    for (int i=0; i < len ; i++) {
        printf("%d,", nums[i]);
    }
}

int main(int argc, char* argv[])
{
    int num[] = {0,0,2,3};
    int len = sizeof(num)/sizeof(int);
    moveZeroes(num, len);

	return 0;
}