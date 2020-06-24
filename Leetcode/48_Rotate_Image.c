/*
48、给定一个 n × n 的二维矩阵表示一个图像。

将图像顺时针旋转 90 度。

说明：

你必须在原地旋转图像，这意味着你需要直接修改输入的二维矩阵。请不要使用另一个矩阵来旋转图像。

示例 1:
给定 matrix = 
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

原地旋转输入矩阵，使其变为:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]

示例 2:
给定 matrix =
[
  [ 5, 1, 9,11],
  [ 2, 4, 8,10],
  [13, 3, 6, 7],
  [15,14,12,16]
], 

原地旋转输入矩阵，使其变为:
[
  [15,13, 2, 5],
  [14, 3, 4, 1],
  [12, 6, 8, 9],
  [16, 7,10,11]
]


注解：
1、主要难点在于如何原地旋转矩阵。

2、找到像素点索引的旋转一般表达式，四元素组
例如元素 (i, j) 对应的四个位置分别是：(i, j)、(N-1-j, i)、(N-1-i, N-1-j)、(j, N-1-i)

3、如果 n 是偶数的话，这相当于把矩阵均分成四块，每块的元素个数是 ( n / 2 ) * ( n / 2 )。如果 n 是奇数，矩阵的中心元素是不随旋转移动的，而剩下的元素均分成四块，每块的元素个数是「 n / 2 」*「 n / 2 」 。

我们对一块中的所有元素做一次四元素旋转即可。

*/

class Solution {
    public void rotate(int[][] matrix) {
        int N = matrix.length;
        for (int i = 0; i < N/2; i++) {
            for (int j = 0; j < (N+1)/2; j++) {
                int t = matrix[i][j];
                matrix[i][j] = matrix[N-1-j][i];
                matrix[N-1-j][i] = matrix[N-1-i][N-1-j];
                matrix[N-1-i][N-1-j] = matrix[j][N-1-i];
                matrix[j][N-1-i] = t;
            }
        }
    }
}

//时间复杂度O(N^2)，空间复杂度O(1)


//思路2，折纸翻转
/*
翻转的规律
矩阵操作名词说明：
对角线 ：按着左上到右下的对角线翻转 \
反对角线：按着右上到左下的对角线翻转 /
竖轴：按着平行于y轴的中心线翻转 |
横轴： 按着平行于x轴的中心线翻转 ——
90°：矩阵顺时针旋转90°
180°：矩阵顺时针旋转180°
279°：矩阵顺时针旋转270°

矩阵翻转规律如下


对角线 + 竖轴 = 90° 
对角线 + 横轴 = 270°

反对角线 + 竖轴 = 270°
反对角线 + 横轴 = 90°

横轴 + 竖轴 = 180°
*/

/*
解题思路

所以本题可以使用两种方法进行翻转
方法一： 对角线 + 竖轴 = 90°
方法二： 反对角线 + 横轴 = 90°
方法一代码

对角线 + 竖轴 = 90°
*/
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();
        
        // 对于正对角线对称翻转
        for (int i = 0; i < n; i++) {
            for (int j = i; j < m; j++) {
                swap(matrix[i][j], matrix[j][i]);
            }
        }
        // 竖轴镜像操作
        for (int i = 0; i < n; i++) {
            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};

//方法二代码
//反对角线 + 横轴 = 90°

class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();
        
        // 对于反正对角线对称翻转
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n-1-i; j++) {
                swap(matrix[i][j], matrix[n-1-j][n-1-i]);
            }
        }
        // 横轴镜像操作
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n/2; i++) {
                swap(matrix[i][j], matrix[n-1-i][j]);
            }
        }
    }
};


class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n=matrix.size();
        for(int i=0;i<n;++i){   //倒转每行
            reverse(matrix[i].begin(),matrix[i].end());
        }
        for(int i=0;i<n;++i){   //沿右对角线对折
            for(int j=0;i+j<n-1;++j){
                swap(matrix[i][j],matrix[n-1-j][n-1-i]);
            }
        }
    }
};

