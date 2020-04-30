#include <stdio.h>

int binarySearch(int* nums, int size, int target) {
    int left = 0; 
    int right = size - 1; // 注意

    while(left <= right) { // 注意
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
            return mid; 
        else if (nums[mid] < target)
            left = mid + 1; // 注意
        else if (nums[mid] > target)
            right = mid - 1; // 注意
        }
    return -1;
}

int binarySearch_leftbound(int* nums, int size, int target) {
    int left = 0; 
    int right = size; // 注意

    while(left < right) { // 注意
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
            right = mid; 
        else if (nums[mid] < target)
            left = mid + 1; // 注意
        else if (nums[mid] > target)
            right = mid; // 注意
        }
    return left;
}

int binarySearch_rightbound(int* nums, int size, int target) {
    int left = 0; 
    int right = size; // 注意

    while(left < right) { // 注意
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
            left = mid + 1; //不要立即返回，而是增大「搜索区间」的下界 left，使得区间不断向右收缩，达到锁定右侧边界的目的。
        else if (nums[mid] < target)
            left = mid + 1; // 注意
        else if (nums[mid] > target)
            right = mid; // 注意
        }
    return left-1;
}


int main(int argc, char* argv[])
{
    int nums[] = {1,1,2,3,4,5,5,5,6,7,8,8};
    int size = sizeof(nums)/sizeof(int);
	
	int target = 8;

    int ret  = 0;
	
	ret = binarySearch(nums, size, target);//普通二分查找是返回不固定的位置
	printf("ret = %d\n", ret);
	
	ret = binarySearch_leftbound(nums, size, target);//返回左边界
	printf("left = %d\n", ret);
	
	ret = binarySearch_rightbound(nums, size, target);//返回右边界
	printf("right = %d\n", ret);

    return 0;
}


/*
Knuth:
Although the basic idea of binary search is comparatively straightforward, the details can be surprisingly tricky...

思路很简单，细节是魔鬼。


第一个，最基本的二分查找算法：因为我们初始化 right = nums.length - 1
所以决定了我们的「搜索区间」是 [left, right]
所以决定了 while (left <= right)
同时也决定了 left = mid+1 和 right = mid-1

因为我们只需找到一个 target 的索引即可
所以当 nums[mid] == target 时可以立即返回第二个，寻找左侧边界的二分查找：因为我们初始化 right = nums.length
所以决定了我们的「搜索区间」是 [left, right)
所以决定了 while (left < right)
同时也决定了 left = mid + 1 和 right = mid

因为我们需找到 target 的最左侧索引
所以当 nums[mid] == target 时不要立即返回
而要收紧右侧边界以锁定左侧边界第三个，寻找右侧边界的二分查找：因为我们初始化 right = nums.length
所以决定了我们的「搜索区间」是 [left, right)
所以决定了 while (left < right)
同时也决定了 left = mid + 1 和 right = mid

因为我们需找到 target 的最右侧索引
所以当 nums[mid] == target 时不要立即返回
而要收紧左侧边界以锁定右侧边界

又因为收紧左侧边界时必须 left = mid + 1
所以最后无论返回 left 还是 right，必须减一

*/