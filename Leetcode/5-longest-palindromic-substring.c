/*
5. 最长回文子串
给定一个字符串 s，找到 s 中最长的回文子串。你可以假设 s 的最大长度为 1000。

示例 1：

输入: "babad"
输出: "bab"
注意: "aba" 也是一个有效答案。
示例 2：

输入: "cbbd"
输出: "bb"

注解：

一
寻找回文串的问题核心思想是：从中间开始向两边扩散来判断回文串。

for 0 <= i < len(s):
    找到以 s[i] 为中心的回文串
    找到以 s[i] 和 s[i+1] 为中心的回文串
    更新答案

时间复杂度 O(N^2)，空间复杂度 O(1)。

二  DP动态规划

时间复杂度一样，但是空间复杂度至少要 O(N^2) 来存储 DP table。这道题是少有的动态规划非最优解法的问题。

三  Manacher's Algorithm（马拉车算法）

时间复杂度只需要 O(N)，不过该解法比较复杂，我个人认为没必要掌握。

*/


string palindrome(string& s, int l, int r)
{
    //防止索引越界
    while(l >= 0 && r < s.size() && s[l] == s[r])
    {
        //向两边展开
        l--;r--;
    }
    //返回以s[l]和s[r]为中心的最长回文串
    return s.substr(l+1, r-l-1);
}

string longestPalindrome(string s)
{
    string res;
    for(int i = 0; i < s.size(); i++)
    {
        //以s[i]为中心的最长回文子串
        string s1 = palindrome（s, i, i）
        //以s[i]和s[i+1]为中心的最长回文子串
        string s2 = palindrome（s, i, i+1）

        res = res.size() > s1.size() ? res : s1;
        res = res.size() > s2.size() ? res : s2;
    }
    
    return res;
}