/*
79. ��������
����һ����ά�����һ�����ʣ��ҳ��õ����Ƿ�����������С�

���ʱ��밴����ĸ˳��ͨ�����ڵĵ�Ԫ���ڵ���ĸ���ɣ����С����ڡ���Ԫ������Щˮƽ���ڻ�ֱ���ڵĵ�Ԫ��ͬһ����Ԫ���ڵ���ĸ�������ظ�ʹ�á�

 

ʾ��:

board =
[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

���� word = "ABCCED", ���� true
���� word = "SEE", ���� true
���� word = "ABCB", ���� false
 

��ʾ��

board �� word ��ֻ������д��СдӢ����ĸ��
1 <= board.length <= 200
1 <= board[i].length <= 200
1 <= word.length <= 10^3


ע�⣺
1.DFS����ͨ����¼��ǰλ�����ﵽ����Ŀ��

2.��visited ������Է�ֹ�ظ��������޷������ݹ�

3.ע���֦
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
                //visit�����¼�������Ƿ񱻷��ʹ�  
                vector<vector<bool>> visit(row, vector<bool>(col, false));  
                bool res = dfs(board, visit, i, j, word, 0);  
                if (res == true)  
                    return true;  
            }  
        }  
        return false;  
    }  
    //index��ʾ���ǵ�ǰ̽�����ǵڼ�����  
    bool dfs(vector<vector<char>>& b, vector<vector<bool>>& visit, int x, int y,                //���������ÿһ��  
        string s, int index)  
    {  
        int row = b.size();  
        int col = b[0].size();  
        if (index == s.length())  
            return true;  
        //���¼������ ���ٽ��д���  
        //1������Խ��  
        //2���ýڵ��Ѿ����ʹ�  
        //3��indexλ�õ��ַ����ַ����е��ַ�����  
        else if (x < 0 || x >= row || y < 0 || y >= col   //1  
                 || visit[x][y] == true                   //2  
                 || s[index]!=b[x][y])                    //3  
            return false;  
        else  
        {  
            visit[x][y] = true;  
            //��xy��������Χ����̽��  
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
    int direction[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};        //��������4������
    void serach_board(int i,int j,int level,string word,vector<vector<char>>& board,vector<vector<int>>& visited)
    {
        if(res)
            return;
        if(level==word.length())         //�Ƚ�����˵
            res = true;
        if(i<0||i>=board.size()||j<0||j>=board[0].size())
            return;
        if(visited[i][j])                //��Щ���൱�ڼ�֦�ˣ��������������Ǻܻ�
            return;
        visited[i][j]=1;                 //�����Ŵ����������
        if(board[i][j]==word[level])     //������������Ϳ��Լ���������ȥ
        {
            for(int k = 0;k<4;k++)
            {
                int new_i = i+direction[k][0];
                int new_j = j+direction[k][1];
               /* if(new_i>=0&&new_i<board.size()&&new_j>=0&&new_j<board[0].size())
                {
                    //����߽������Ļ�
                    //���������û��return�Ļ�����ô�õ��Ľ���Ƿ��ز��˵�
                    serach_board(new_i,new_j,level+1,word,board,visited); 
                }
                */
                serach_board(new_i,new_j,level+1,word,board,visited); 
            }
        }
        visited[i][j]=0;                           //һ��Ҫ��֤����ʹ�������
        //return false;              
    }
    bool exist(vector<vector<char>>& board, string word) {
        // ����������
        //��ά�����ÿ���㶼������Ϊ�����Ŀ�ʼ
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
        return false;            //���ǰ�涼ô�з���true����ô�ͷ���false
    };
};