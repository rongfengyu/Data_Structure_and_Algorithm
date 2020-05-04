/*
66. 加一
给定一个由整数组成的非空数组所表示的非负整数，在该数的基础上加一。

最高位数字存放在数组的首位， 数组中每个元素只存储单个数字。

你可以假设除了整数 0 之外，这个整数不会以零开头。

示例 1:

输入: [1,2,3]
输出: [1,2,4]
解释: 输入数组表示数字 123。
示例 2:

输入: [4,3,2,1]
输出: [4,3,2,2]
解释: 输入数组表示数字 4321。


考虑到两种情况：
1、普通情况，除9之外的数字加1。
2、特殊情况，9加1。（因为9加1需要进位）

注解:全部是9的特殊情况，需要扩充数组；子函数中malloc分配内存与返回，主函数中释放
*/


#include <stdio.h>
#include <stdlib.h>

int plusOne(int digits[], int len, int **out_p)
{
    for(int i=len-1; i>=0; i--) 
    {
        if (digits[i] < 9)
        { // 当前位置不用进位，+1，然后直接返回
            digits[i]=digits[i]+1;
            *out_p = digits;
            return len;
        } else 
        { // 要进位，当前位置置0
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