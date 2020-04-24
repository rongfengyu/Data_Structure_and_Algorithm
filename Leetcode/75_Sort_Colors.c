/*
75. ��ɫ���ࣨ�����죩

����һ��������ɫ����ɫ����ɫ��һ�� n ��Ԫ�ص����飬ԭ�ض����ǽ�������ʹ����ͬ��ɫ��Ԫ�����ڣ������պ�ɫ����ɫ����ɫ˳�����С�

�����У�����ʹ������ 0�� 1 �� 2 �ֱ��ʾ��ɫ����ɫ����ɫ��

ע��:
����ʹ�ô�����е����������������⡣

ʾ��:

����: [2,0,2,1,1,0]
���: [0,0,1,1,2,2]
���ף�

һ��ֱ�۵Ľ��������ʹ�ü������������ɨ���㷨��
���ȣ����������0��1 �� 2 Ԫ�صĸ�����Ȼ����0��1��2��������д��ǰ���顣
�������һ����ʹ�ó����ռ��һ��ɨ���㷨��


ע�⣺
1�������㷨������·������������߼�����������ɨ�裩��

2����ָ�뷨����¼0��2��λ�ã��Լ�ʹ��һָ������������������Ԫ��λ�ã�ʱ��O(N)��

3����Ư���Ľⷨ��ƽ�Ʋ���
*/

#include <stdio.h>
	
void swap(int* a, int *b)
{
	int tmp = *b;
	*b = *a;
	*a = tmp;
}

void sortColors(int* nums, int numsSize)
{
	int left = 0;
	int right = numsSize-1;
	int i = 0;
	while(i <= right)
	{
		if(nums[i] == 0)
		{
			swap(&nums[left], &nums[i]);
			left ++;
			i ++;
		}
		else if(nums[i] == 1)
		{
			i ++;
		}    
		else
		{
			swap(&nums[i], &nums[right]);
			right --;
		}
	}
}

void sortColors2(int* nums, int numsSize)
{
	int i = -1;
	int j = -1;
	int k = -1;
	int p = 0;
	for(p = 0; p < numsSize; p ++)
	{
		//���ݵ�i�����֣�Ų��0~i-1����
		if(nums[p] == 0)
		{
			nums[++k] = 2;    //2����Ų
			nums[++j] = 1;    //1����Ų
			nums[++i] = 0;    //0����Ų
		}
		else if(nums[p] == 1)
		{
			nums[++k] = 2;
			nums[++j] = 1;
		}
		else
			nums[++k] = 2;
	}

}

int main(int argc, char* argv[])
{
    int nums[] = {2,0,1,1,0,0};
    int size = sizeof(nums)/sizeof(int);

    sortColors2(nums, size);
	
	int i = 0;
	for(i = 0; i < size; i++)
		printf("%d ", nums[i]);

    return 0;
}