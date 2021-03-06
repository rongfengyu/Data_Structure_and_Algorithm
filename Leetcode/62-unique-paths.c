/*
62. 不同路径
一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为“Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。

问总共有多少条不同的路径？

示例?1:

输入: m = 3, n = 2
输出: 3
解释:
从左上角开始，总共有 3 条路径可以到达右下角。
1. 向右 -> 向右 -> 向下
2. 向右 -> 向下 -> 向右
3. 向下 -> 向右 -> 向右
示例?2:

输入: m = 7, n = 3
输出: 28

提示：

1 <= m, n <= 100
题目数据保证答案小于等于 2 * 10 ^ 9

注解:

标准的动态规划DP类型题目

全局限制性的路径问题（本题的限制条件是只能向右或者向下前进），可以考虑使用动态规划的问题进行求解

状态转移表达式为dp[i][j]=dp[i-1][j] + dp[i][j-1], 其中i,j >= 1, i <= m, j <= n


分析题目发现，到达一个点的方法只能是从这个点的上方或者左方。所以问题的解可以尝试通过左边的点的解和上边的解的结合得到。  

如何结合得到？如果从开始到达(i, j)这个点的上方点(i - 1, j)的路径数为m, 从开始到达(i,j)点的左方点(i, j - 1)的路径数为n，显然可以得到到达(i, j)的路径数就是 m + n。

也就是到达上方点的路径数加上到达左方点的路径数也就是到达上方点的路径数加上到达左方点的路径数。

由于到达每个左边都有一个路径数，不妨设开始点到达(i, j)点的路径数为dpi。 那么，根据上边的分析有开始点到达(i, j)点的路径数为dp[i][j] = dp[i - 1][j] + dp[i][j - 1]。这个方程是显然成立的。

时间复杂度: O(mn)，表格中每个位置进行一次计算即可。

正确性证明

以上做法使用的动态规划的思想，下面证明一下本题可以使用动态规划的思想。

一个问题要想使用动态规划进行求解，就需要满足两个条件：（１）具有最优子结构（２）无后效性。

在本题中，从开始点到达某点的路径数可以看做是从开始点到达某点最多的路径数。
如果求从开始点到达某点最多的路径数，需要要求已知开始点到达它上方点的最多路径数和开始点到达它下方点的最多路径数，以此进行转移，因此满足条件（１）；

对于条件（２），表明当前决策，也就是dp[i][j] = dp[i - 1][j] + dp[i][j - 1]是不需要通过后边的状态得到的或者说不受后边状态（比如dpi + 1）的影响，显然是满足的。

所以本题可以根据上述状态转移方程进行求解. 

*/


class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>>dp(m + 1, vector<int>(n + 1));
        for(int i = 0; i <= m; i ++) dp[i][0] = 0;
        for(int j = 0; j <= n; j ++) dp[0][j] = 0;
        
        for(int i = 1; i <= m; i ++) {
            for(int j = 1; j <= n; j ++) {
                if(i == 1 && j == 1) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                }
            }
        }
        return dp[m][n];
    }
};

//第二种写法，初始值定义不一样
//go
func uniquePaths(m int, n int) int {
    dp := make([][]int, m)
    for i := 0; i < m; i++ {
         dp[i] = make([]int, n)
    }
    for i := 0; i < m; i++ {
        dp[i][0] = 1
    }
    for j := 0; j < n; j++ {
        dp[0][j] = 1
    }
    for i := 1; i < m; i++ {
        for j := 1; j < n; j++ {
          dp[i][j] = dp[i-1][j] + dp[i][j-1]
        }
    }
    return dp[m-1][n-1]
}

//使用滚动数组优化
//go
func uniquePaths(m int, n int) int {
    dp := make([]int, n)
    for j := 0; j < n; j++ {
       dp[j] = 1
    }
    for i := 1; i < m; i++ {
        for j := 1; j < n; j++ {
            //注意，这里dp[j-1]已经是新一行的数据了，而dp[j]仍然是上一行的数据
            dp[j] += dp[j - 1]
       }
    }
    return dp[n-1]
}
