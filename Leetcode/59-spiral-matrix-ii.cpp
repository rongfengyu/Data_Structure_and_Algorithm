/*
59. 螺旋矩阵 II
给定一个正整数 n，生成一个包含 1 到 n^2 所有元素，且元素按顺时针顺序螺旋排列的正方形矩阵。

示例:

输入: 3
输出:
[
 [ 1, 2, 3 ],
 [ 8, 9, 4 ],
 [ 7, 6, 5 ]
]


题解思路
先清空矩阵，把数字从小到大填进去，那么就是一直往前走的一条线，每次这条线到尽头或者到一个填过的点就右转（初始在[0,0]位置方向向右）。
那么就可以直接拿x方向的增量和y方向的增量来模拟，每次试着从上一次的增量方向前进，如果到了边界外或者到过的点，就修正方向（右转），并继续前进，直至填的数字大于n*n。

如n=2的情况，初始化：

[0, 0],
[0, 0]
当前位置[0,0]，x增量0，y增量1，填的数字是1
填充，前进一步：

[1, 0],
[0, 0]
当前位置[0,1]，x增量0，y增量1，填的数字是2
填充，试着前进一步，发现出了边界，修正方向为向下。重新前进一步：

[1, 2],
[0, 0]
当前位置[1,1]，x增量1，y增量0，填的数字是3
填充，试着前进一步，发现出了边界，修正方向为向左。重新前进一步：

[1, 2],
[0, 3]
当前位置[1,0]，x增量0，y增量-1，填的数字是4
填充，填完后填的数字变成了5，大于2*2，结束。

[1, 2],
[4, 3]


*/

class Solution {
public:
   void turn (int &dis_x, int &dis_y) {
     if (dis_x == 1) {
            dis_x = 0, dis_y = -1;
        }
        else if (dis_x == -1) {
            dis_x = 0, dis_y = 1;
        }
        else if (dis_y == 1) {
            dis_x = 1, dis_y = 0;
        }
        else if (dis_y == -1) {
            dis_x = -1, dis_y = 0;
        }
    }
    
    void go (int x, int y, int dis_x, int dis_y, int num, vector<vector<int> > &ans, int &n) {
        if (num > n*n || ans[x][y]) return ;
        ans[x][y] = num++;
        int xx = x+dis_x, yy = y+dis_y;
        if (xx >= n || yy >= n || xx < 0 || yy < 0 || ans[xx][yy]) 
         turn (dis_x, dis_y);
        go (x+dis_x, y+dis_y, dis_x, dis_y, num, ans, n);
    }
    
    vector<vector<int> > generateMatrix(int n) {
        vector<vector<int> > ans (n, vector<int> (n, 0));
        go (0, 0, 0, 1, 1, ans, n);
        return ans;
    }
};
