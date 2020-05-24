/*
312. 戳气球
有 n 个气球，编号为0 到 n-1，每个气球上都标有一个数字，这些数字存在数组 nums 中。

现在要求你戳破所有的气球。每当你戳破一个气球 i 时，你可以获得 nums[left] * nums[i] * nums[right] 个硬币。 这里的 left 和 right 代表和 i 相邻的两个气球的序号。注意当你戳破了气球 i 后，气球 left 和气球 right 就变成了相邻的气球。

求所能获得硬币的最大数量。

说明:

你可以假设 nums[-1] = nums[n] = 1，但注意它们不是真实存在的所以并不能被戳破。
0 ≤ n ≤ 500, 0 ≤ nums[i] ≤ 100
示例:

输入: [3,1,5,8]
输出: 167 
解释: nums = [3,1,5,8] --> [3,5,8] -->   [3,8]   -->  [8]  --> []
     coins =  3*1*5      +  3*5*8    +  1*3*8      + 1*8*1   = 167


解释: 

WX labuladong
https://mp.weixin.qq.com/s/I0yo0XZamm-jMpG-_B3G8g

解法一 —— 回溯法

简单粗暴，但是相应的，算法的效率非常低。
这个解法等同于全排列，所以时间复杂度是阶乘级别，非常高，
题目说了nums的大小n最多为 500，所以回溯算法肯定是不能通过所有测试用例的。

解法二 —— DP

DP的关键点是定义DP数组的含义
写出状态转移方程

这个问题中我们每戳破一个气球nums[i]，得到的分数和该气球相邻的气球nums[i-1]和nums[i+1]是有相关性的。
如果想用动态规划，必须巧妙地定义dp数组的含义，避免子问题产生相关性，才能推出合理的状态转移方程。

1、在首尾两端添加两个虚拟气球
现在气球的索引变成了从1到n，points[0]和points[n+1]可以认为是两个「虚拟气球」。

2、定义dp数组的含义

dp[i][j] = x表示，戳破气球i和气球j之间（开区间，不包括i和j）的所有气球，可以获得的最高分数为x。

那么根据这个定义，题目要求的结果就是dp[0][n+1]的值，而 base case 就是dp[i][j] = 0，其中0 <= i <= n+1, j <= i+1，因为这种情况下，开区间(i, j)中间根本没有气球可以戳。

3、转移方程
根据刚才对dp数组的定义，如果最后一个戳破气球k，dp[i][j]的值应该为：

dp[i][j] = dp[i][k] + dp[k][j] 
         + points[i]*points[k]*points[j]

for (int i = ...; ; )//从下往上遍历
    for (int j = ...; ; )//从下往上遍历
        for (int k = i + 1; k < j; k++) {
            dp[i][j] = Math.max(  // 择优做选择，使得 dp[i][j] 最大
                dp[i][j], 
                dp[i][k] + dp[k][j] + points[i]*points[j]*points[k]
            );
return dp[0][n+1];

*/

//DP，此题的关键是分析状态转移方程
int maxCoins(int[] nums) {
    int n = nums.length;
    // 添加两侧的虚拟气球
    int[] points = new int[n + 2];
    points[0] = points[n + 1] = 1;//添加首尾虚拟气球
    for (int i = 1; i <= n; i++) {
        points[i] = nums[i - 1];//重新排列数组，添加首尾
    }
    // base case 已经都被初始化为 0
    int[][] dp = new int[n + 2][n + 2];
    // 开始状态转移
    // i 应该从下往上
    for (int i = n; i >= 0; i--) {
        // j 应该从左往右
        for (int j = i + 1; j < n + 2; j++) {
            // 最后戳破的气球是哪个？
            for (int k = i + 1; k < j; k++) {
                // 择优做选择
                dp[i][j] = Math.max(
                    dp[i][j], 
                    dp[i][k] + dp[k][j] + points[i]*points[j]*points[k]
                );
            }
        }
    }
    return dp[0][n + 1];
}
