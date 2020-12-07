/*
64. 最小路径和
给定一个包含非负整数的 m x n 网格，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

说明：每次只能向下或者向右移动一步。

示例:

输入:
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
输出: 7
解释: 因为路径 1→3→1→1→1 的总和最小。


注解:

标准的动态规划DP类型题目

全局限制性的路径问题（本题的限制条件是只能向右或者向下前进），可以考虑使用动态规划的问题进行求解

状态转移表达式为dp[i][j]=min(dp[i-1][j], dp[i][j-1]) + grid[i][j], 其中i,j >= 1, i <= m, j <= n


分析本题，到达点（m, n）的路径一定经过（m - 1, n）或者（m, n - 1）。
也就是说，到达（m, n）的最小路径（即所有路径中路径上所有数值和最小的路径）一定是到达（m - 1, n）的最小路径或者到达（m, n - 1）的最小路径。

因此，题目可以转化为求到达（m - 1, n）的最小路径和到达（m, n - 1）的最小路径。一次递推下去。就形成了最优子结构性质。

依据上边的分析，可以看到，从起点到达每个点的最小路径值就可以当做一个状态，
如果设dp[i][j]是从起始点（1, 1）到点（i, j）的最小路径值，那么状态转移方程也就是dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j], 其中i,j >= 1, i <= m, j <= n。

显然这个状态是无后效性的（即本状态不受本点之后的状态的影响）。
通过简单的二重循环就可以对以上方程求解。

时间复杂度: O(mn)，二重循环，对表格中每一个位置求解。


*/


class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>>dp(m, vector<int>(n));
        dp[0][0] = grid[0][0];
        for(int i = 1; i < m; i ++)  dp[i][0] = dp[i - 1][0] + grid[i][0];//初始化首列
        for(int j = 1; j < n; j ++)  dp[0][j] = dp[0][j - 1] + grid[0][j];//初始化首行
        for(int i = 1; i < m; i ++) {
            for(int j = 1; j < n; j ++) {
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }
        return dp[m - 1][n - 1];
    }
};


//C语言实现
#include<stdio.h>
#include<stdlib.h>

int minPathSum(int** grid, int gridSize, int gridColSize)
{
    int rows = gridSize;
    int cols = gridColSize;
    if(rows==0 || cols==0)
        return 0;

    int dp[rows][cols];
    dp[0][0] = grid[0][0];
    int i,j;
    for(i = 1; i < rows; i++)
        dp[i][0] = dp[i-1][0] + grid[i][0];
    
    for(j = 1; j < cols; j++)
        dp[0][j] = dp[0][j-1] + grid[0][j];
    
    for(i = 1; i < rows; i++)
        for(j = 1; j < cols; j++)
		{
			dp[i][j] = min3(dp[i-1][j-1], dp[i-1][j], dp[i][j-1]) + grid[i][j];//可以从上、左、斜左上方进行移动
			//dp[i][j] = min2(dp[i-1][j], dp[i][j-1]) + grid[i][j];//可以从上、左进行移动
		}
            
		

    return dp[rows-1][cols-1];
}

int min3(int x, int y, int z)
{
   if (x < y)
      return (x < z)? x : z;
   else
      return (y < z)? y : z;
}

int min2(int x, int y)
{
    return (x < y)? x : y;
}

int main()
{
    int cost[][3] = { {1, 2, 3},
                      {4, 8, 2},
                      {1, 5, 3},{1,1,1} };

	int row = sizeof(cost)/sizeof(cost[0]);
	int col = sizeof(cost[0])/sizeof(cost[0][0]);
	
	//使用指针数组将二维数组转化为二维指针进行传递，更加灵活
    int** ppcost = (int **)malloc(sizeof(int *)* row);//或者int *pcost[row];
    int i=0;
    for(i=0; i < row; i++)
        ppcost[i] = (int *)(cost + i);

   printf(" %d \n", minPathSum(ppcost, row, col));
   return 0;
}

