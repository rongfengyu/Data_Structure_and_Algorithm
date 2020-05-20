/*
70. 爬楼梯
假设你正在爬楼梯。需要 n 阶你才能到达楼顶。

每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

注意：给定 n 是一个正整数。

示例 1：

输入： 2
输出： 2
解释： 有两种方法可以爬到楼顶。
1.  1 阶 + 1 阶
2.  2 阶
示例 2：

输入： 3
输出： 3
解释： 有三种方法可以爬到楼顶。
1.  1 阶 + 1 阶 + 1 阶
2.  1 阶 + 2 阶
3.  2 阶 + 1 阶


注解：
1、和青蛙跳台阶一样，很经典的dp类型题，其推导公式和求解斐波拉契数一致

这个问题可以被分解为一些包含最优子结构的子问题,它的最优解可以从其子问题的最优解来有效地构建,因此我们可以使用动态规划解决这个问题。
dp[i] = dp[i - 1] + dp[i - 2]

2、关键在优化以及修枝减少重复运算
首先，使用长度为N的数组dp[N]来存储过程的中间值，可减少重复计算，时间复杂度O(N)，空间复杂度O(N)
然后，当前值依赖前两个值，使用两个变量保存即可，可减少存储空间，时间复杂度O(N)，空间复杂度O(1)

*/

#include <stdio.h>
	
int climbStairs(int n)
{
    if(n <= 1)
        return n;

    int first = 1;
    int second = 2;
    int third;

    int i;
    for(i=3; i<=n; i++)
    {
        third = first + second;
        first = second;
        second = third;
    }

    return second;//为什么是返回second，因为n=2时
}

int main(int argc, char* argv[])
{
    printf("%d ", climbStairs(5));

    return 0;
}



class Solution {
public:
    int climbStairs(int n) {
        vector<int>dp(n + 1);
        dp[0] = 1;
        dp[1] = 1;
        for(int i = 2; i <= n; i ++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }
};