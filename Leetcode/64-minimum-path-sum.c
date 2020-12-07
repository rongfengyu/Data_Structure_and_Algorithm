/*
64. ��С·����
����һ�������Ǹ������� m x n �������ҳ�һ�������Ͻǵ����½ǵ�·����ʹ��·���ϵ������ܺ�Ϊ��С��

˵����ÿ��ֻ�����»��������ƶ�һ����

ʾ��:

����:
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
���: 7
����: ��Ϊ·�� 1��3��1��1��1 ���ܺ���С��


ע��:

��׼�Ķ�̬�滮DP������Ŀ

ȫ�������Ե�·�����⣨���������������ֻ�����һ�������ǰ���������Կ���ʹ�ö�̬�滮������������

״̬ת�Ʊ��ʽΪdp[i][j]=min(dp[i-1][j], dp[i][j-1]) + grid[i][j], ����i,j >= 1, i <= m, j <= n


�������⣬����㣨m, n����·��һ��������m - 1, n�����ߣ�m, n - 1����
Ҳ����˵�����m, n������С·����������·����·����������ֵ����С��·����һ���ǵ��m - 1, n������С·�����ߵ��m, n - 1������С·����

��ˣ���Ŀ����ת��Ϊ�󵽴m - 1, n������С·���͵��m, n - 1������С·����һ�ε�����ȥ�����γ��������ӽṹ���ʡ�

�����ϱߵķ��������Կ���������㵽��ÿ�������С·��ֵ�Ϳ��Ե���һ��״̬��
�����dp[i][j]�Ǵ���ʼ�㣨1, 1�����㣨i, j������С·��ֵ����ô״̬ת�Ʒ���Ҳ����dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j], ����i,j >= 1, i <= m, j <= n��

��Ȼ���״̬���޺�Ч�Եģ�����״̬���ܱ���֮���״̬��Ӱ�죩��
ͨ���򵥵Ķ���ѭ���Ϳ��Զ����Ϸ�����⡣

ʱ�临�Ӷ�: O(mn)������ѭ�����Ա����ÿһ��λ����⡣


*/


class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>>dp(m, vector<int>(n));
        dp[0][0] = grid[0][0];
        for(int i = 1; i < m; i ++)  dp[i][0] = dp[i - 1][0] + grid[i][0];//��ʼ������
        for(int j = 1; j < n; j ++)  dp[0][j] = dp[0][j - 1] + grid[0][j];//��ʼ������
        for(int i = 1; i < m; i ++) {
            for(int j = 1; j < n; j ++) {
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }
        return dp[m - 1][n - 1];
    }
};


//C����ʵ��
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
			dp[i][j] = min3(dp[i-1][j-1], dp[i-1][j], dp[i][j-1]) + grid[i][j];//���Դ��ϡ���б���Ϸ������ƶ�
			//dp[i][j] = min2(dp[i-1][j], dp[i][j-1]) + grid[i][j];//���Դ��ϡ�������ƶ�
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
	
	//ʹ��ָ�����齫��ά����ת��Ϊ��άָ����д��ݣ��������
    int** ppcost = (int **)malloc(sizeof(int *)* row);//����int *pcost[row];
    int i=0;
    for(i=0; i < row; i++)
        ppcost[i] = (int *)(cost + i);

   printf(" %d \n", minPathSum(ppcost, row, col));
   return 0;
}

