/*
200. 岛屿数量
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。

岛屿总是被水包围，并且每座岛屿只能由水平方向或竖直方向上相邻的陆地连接形成。

此外，你可以假设该网格的四条边均被水包围。

 

示例 1:

输入:
11110
11010
11000
00000
输出: 1
示例 2:

输入:
11000
11000
00100
00011
输出: 3
解释: 每座岛屿只能由水平和/或竖直方向上相邻的陆地连接而成。

注解：
1、dfs问题，递归的思想，加上条件判断（1？0？）

2、查找过的数组元素标记为2，以示区别

3、按四个方向进行dfs，注意边界条件、已经遍历过的元素判断

*/

class Solution {
    public int numIslands(char[][] grid) {
        if (grid.length == 0 || grid[0].length == 0) {
            return 0;
        }

        int count = 0;
        for (int r = 0; r < grid.length; r++) {
            for (int c = 0; c < grid[0].length; c++) {
                if (grid[r][c] == '1') {
                    dfs(grid, r, c);
                    count++;//在此处进行计数
                }
            }
        }
        return count;
    }

    void dfs(char[][] grid, int r, int c) {
        if (!(0 <= r && r < grid.length && 0 <= c && c < grid[0].length)) {
            return;
        }
        if (grid[r][c] != '1') {
            return;
        }
        grid[r][c] = '2';
        dfs(grid, r - 1, c);//四个方向是必然的，因为你不知道前一个点是怎样过来的，最多重复判断了下
        dfs(grid, r + 1, c);
        dfs(grid, r, c - 1);
        dfs(grid, r, c + 1);
    }
}

//时间复杂度O(N^2)，空间复杂度O(N^2)