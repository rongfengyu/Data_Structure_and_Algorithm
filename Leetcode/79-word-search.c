/*
79. 单词搜索
给定一个二维网格和一个单词，找出该单词是否存在于网格中。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

 

示例:

board =
[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

给定 word = "ABCCED", 返回 true
给定 word = "SEE", 返回 true
给定 word = "ABCB", 返回 false
 

提示：

board 和 word 中只包含大写和小写英文字母。
1 <= board.length <= 200
1 <= board[i].length <= 200
1 <= word.length <= 10^3


注解：
1.DFS可以通过记录当前位置来达到深搜目的

2.用visited 数组可以防止重复搜索，无法结束递归

3.注意剪枝
*/

class Solution {  
public:  
    bool exist(vector<vector<char>>& board, string word) {  
        int row = board.size();  
        int col = board[0].size();  
        for (int i = 0; i < row; i++)  
        {  
            for (int j = 0; j < col; j++)  
            {  
                //visit数组记录该数组是否被访问过  
                vector<vector<bool>> visit(row, vector<bool>(col, false));  
                bool res = dfs(board, visit, i, j, word, 0);  
                if (res == true)  
                    return true;  
            }  
        }  
        return false;  
    }  
    //index表示的是当前探索的是第几个词  
    bool dfs(vector<vector<char>>& b, vector<vector<bool>>& visit, int x, int y,                //遍历数组的每一点  
        string s, int index)  
    {  
        int row = b.size();  
        int col = b[0].size();  
        if (index == s.length())  
            return true;  
        //以下几种情况 不再进行处理  
        //1、数组越界  
        //2、该节点已经访问过  
        //3、index位置的字符与字符串中的字符不符  
        else if (x < 0 || x >= row || y < 0 || y >= col   //1  
                 || visit[x][y] == true                   //2  
                 || s[index]!=b[x][y])                    //3  
            return false;  
        else  
        {  
            visit[x][y] = true;  
            //从xy出发向周围进行探索  
            bool x_1y = dfs(b, visit, x - 1, y, s, index + 1);  
            bool x1y = dfs(b, visit, x + 1, y, s, index + 1);  
            bool xy_1 = dfs(b, visit, x, y - 1, s, index + 1);  
            bool xy1 = dfs(b, visit, x, y + 1, s, index + 1);  
  
            if (x_1y || x1y || xy_1 || xy1)  
                return true;  
            else  
            {  
                visit[x][y] = false;  
                return false;  
            }  
        }  
    }  
};  


class Solution {
public:
    bool res = false;
    //int visited[board.size()][board[0].size()];
    int direction[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};        //上下左右4个方向
    void serach_board(int i,int j,int level,string word,vector<vector<char>>& board,vector<vector<int>>& visited)
    {
        if(res)
            return;
        if(level==word.length())         //先进来再说
            res = true;
        if(i<0||i>=board.size()||j<0||j>=board[0].size())
            return;
        if(visited[i][j])                //这些都相当于剪枝了，但是最坏的情况还是很坏
            return;
        visited[i][j]=1;                 //进来才处理这个东西
        if(board[i][j]==word[level])     //如果满足它，就可以继续搜索下去
        {
            for(int k = 0;k<4;k++)
            {
                int new_i = i+direction[k][0];
                int new_j = j+direction[k][1];
               /* if(new_i>=0&&new_i<board.size()&&new_j>=0&&new_j<board[0].size())
                {
                    //满足边界条件的话
                    //如果这下面没有return的话，那么得到的结果是返回不了的
                    serach_board(new_i,new_j,level+1,word,board,visited); 
                }
                */
                serach_board(new_i,new_j,level+1,word,board,visited); 
            }
        }
        visited[i][j]=0;                           //一定要保证的是使他变回来
        //return false;              
    }
    bool exist(vector<vector<char>>& board, string word) {
        // 此题用搜索
        //二维矩阵的每个点都可以作为搜索的开始
        int row = board.size(),col = board[0].size();
        int i,j;
        vector<vector <int> > visited(row ,vector<int>(col,0));
       
        for(i = 0;i<row;i++)
        {
            for(j = 0;j<col;j++)
            {
                serach_board(i,j,0,word,board,visited);
                    if(res)
                    return true;
            }
        }
        return false;            //如果前面都么有返回true，那么就返回false
    };
};