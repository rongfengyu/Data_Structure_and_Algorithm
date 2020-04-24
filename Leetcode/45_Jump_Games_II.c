/*
45. 跳跃游戏 II
给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

你的目标是使用最少的跳跃次数到达数组的最后一个位置。

示例:

输入: [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
说明:

假设你总是可以到达数组的最后一个位置。


注解：
1、每个元素是可以跳跃的最大长度，而不是必须跳跃的长度；

2、需要知道上次能到达的位置，作为这一次的起点。然后需要知道此次能到达的最远的位置，作为下一次的起点。

3、贪心算法：我们在跳的时候会考虑到下一步。根据下一步能跳的最远的贪心策略，来进行每一步的选择。
扫描数组，以确定当前最远能覆盖的节点。然后继续扫描，直到当前的路程超过了上一次算出的覆盖范围，那么更新覆盖范围，同时更新条数，因为我们是经过了多一跳才能继续前进的。
形象地说，这个是在争取每跳最远的greedy。

*/

#include <stdio.h>
	
int jump(int* nums, int numsSize)
{
    int cnt = 0;
    if (nums == NULL || numsSize == 0) {
        return cnt;
    }
    
    int maxPos = 0; //下次最远能到达的位置
    int right = 0;   //本次次最远能到达的位置
    
    for (int i = 0; i < numsSize - 1; i++) {
        maxPos = (nums[i] + i) > maxPos ? (nums[i] + i) : maxPos;//使用三元操作符来操作变量的变化，是一个很高端的用法
        printf("i=%d, maxPos = %d\n", i, maxPos);

        if (i == right) {
            cnt++;//存在计数滞后

            right = maxPos;
            printf("right = %d\n", right);
        }

    }
    return cnt;
}

int main(int argc, char* argv[])
{
    int nums[] = {2,3,1,1,2,2,1,1};
    int size = sizeof(nums)/sizeof(int);

    int ret = jump(nums, size);
	
	printf("ret = %d ", ret);

    return 0;
}