/*
66. ��һ
����һ����������ɵķǿ���������ʾ�ķǸ��������ڸ����Ļ����ϼ�һ��

���λ���ִ�����������λ�� ������ÿ��Ԫ��ֻ�洢�������֡�

����Լ���������� 0 ֮�⣬��������������㿪ͷ��

ʾ�� 1:

����: [1,2,3]
���: [1,2,4]
����: ���������ʾ���� 123��
ʾ�� 2:

����: [4,3,2,1]
���: [4,3,2,2]
����: ���������ʾ���� 4321��


���ǵ����������
1����ͨ�������9֮������ּ�1��
2�����������9��1������Ϊ9��1��Ҫ��λ��

ע��:ȫ����9�������������Ҫ�������飻�Ӻ�����malloc�����ڴ��뷵�أ����������ͷ�
*/


#include <stdio.h>
#include <stdlib.h>

int plusOne(int digits[], int len, int **out_p)
{
    for(int i=len-1; i>=0; i--) 
    {
        if (digits[i] < 9)
        { // ��ǰλ�ò��ý�λ��+1��Ȼ��ֱ�ӷ���
            digits[i]=digits[i]+1;
            *out_p = digits;
            return len;
        } else 
        { // Ҫ��λ����ǰλ����0
            digits[i] = 0;
        }
    }  

    *out_p=(int *)calloc(len+1, sizeof(int));
    if(*out_p == NULL)
        return 0;

    (*out_p)[0] = 1;
    
    return len+1;
}

int main(int argc, char* argv[])
{
    int* out = NULL;
    int num[] = {6,9,9};
    printf("%p\n", num);
    int len = sizeof(num)/sizeof(int);
    int newlen = plusOne(num, len, &out);

    for (int i=0; i < newlen ; i++) {
        printf("%d,", out[i]);
    }
    printf("%p\n", out);
    if(newlen != len){
        free(out);
        out = NULL;
    }
	return 0;
}