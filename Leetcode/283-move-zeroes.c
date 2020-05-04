/*
283. 移动零
给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

示例:

输入: [0,1,0,3,12]
输出: [1,3,12,0,0]
说明:

必须在原数组上操作，不能拷贝额外的数组。
尽量减少操作次数。

注解：双指针，不改变非零元素相对顺序；不改变元素相对顺序，则不能使用从尾部开始交换元素的方法（leetcode27）
*/

#include <stdio.h>

void moveZeroes(int nums[], int len)
{
    int tmp;
    //快指针遇到非0元素就与慢指针所指的元素交换，最后快指针与慢指针之间的元素都为0
    for(int slow=0,i=0;i<len;++i)
        if(nums[i]!=0 && (slow!=i))//略过以非零元素开始的交换
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