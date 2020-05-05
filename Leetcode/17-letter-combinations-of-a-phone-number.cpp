/*
17. 电话号码的字母组合
给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

示例:

输入："23"
输出：["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
说明:
尽管上面的答案是按字典序排列的，但是你可以任意选择答案输出的顺序。


注解：

回溯算法、dfs

题目要求使用回溯法。

回溯法的过程十分简单，就是按照要求的次序进行搜索（通常是深度优先搜索），得到一个答案或者无法继续搜索时则进行回溯，之后继续搜索，直到穷尽所有可能的要求的次序。 

本题采用递归的方法，搜索过程中记录本次数字代表的字母次序，过程采用深度优先搜索的方法，

每层进行一个数字代表的不同字母的尝试，先尝试一个字母，然后递归这个字母被选定情况下的所有搜索，
之后把这个字母换成这个数字代表的下一个字母，直到搜索完这个字母代表的所有字母，然后返回上一层。

时间复杂度: 最差为O(4^n)。算法的过程实质上等价于枚举，最坏的情况下每个数字代表4个字母，一共有n个数字.  


*/

class Solution {
public:
    void work(string digits, string &temp, vector<string>& result, vector<string>dict) {
        
        int len = temp.length(), lend = digits.length();
        if(len == lend) {
            result.push_back(temp);
            return ;
        }
        
        for(int i = 0; i < dict[digits[len] - '0'].length(); i ++) {
            temp += dict[digits[len] - '0'][i];
            work(digits, temp, result, dict);
            temp.erase(temp.length() - 1);
        }
    }
    
    vector<string> letterCombinations(string digits) {

        vector<string>dict(10);
        dict[2] = "abc";
        dict[3] = "def";
        dict[4] = "ghi";
        dict[5] = "jkl";
        dict[6] = "mno";
        dict[7] = "pqrs";
        dict[8] = "tuv";
        dict[9] = "wxyz";
        
        vector<string>result;
        
        if(digits.length() == 0) {
            return result;
        }
        string temp = "";
        work(digits, temp, result, dict);
        return result;
    }
};