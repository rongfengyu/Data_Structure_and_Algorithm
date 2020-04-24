/*
75. 颜色分类（荷兰旗）

给定一个包含红色、白色和蓝色，一共 n 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。

此题中，我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。

注意:
不能使用代码库中的排序函数来解决这道题。

示例:

输入: [2,0,2,1,1,0]
输出: [0,0,1,1,2,2]
进阶：

一个直观的解决方案是使用计数排序的两趟扫描算法。
首先，迭代计算出0、1 和 2 元素的个数，然后按照0、1、2的排序，重写当前数组。
你能想出一个仅使用常数空间的一趟扫描算法吗？


注解：
1、排序算法：（三路）快速排序或者计数排序（两趟扫描）。

2、三指针法：记录0、2的位置，以及使用一指针进行数组遍历，交换元素位置，时间O(N)；

3、最漂亮的解法：平移插入
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
		//根据第i个数字，挪动0~i-1串。
		if(nums[p] == 0)
		{
			nums[++k] = 2;    //2往后挪
			nums[++j] = 1;    //1往后挪
			nums[++i] = 0;    //0往后挪
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