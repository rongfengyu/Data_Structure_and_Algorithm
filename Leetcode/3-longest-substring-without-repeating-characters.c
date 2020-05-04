/*
3. 无重复字符的最长子串
给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。

示例 1:

输入: "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。

注解：
滑块思想，最大窗口。快慢指针i,j结合hash。

（1）快指针j所在元素不重复，更新max，将快指针j元素在hash表中的标记为出现，后移j

（2）快指针j所在元素重复，慢指针后移，此时将慢指针i元素在hash表中的标记清除。此时并不关心是谁重复，重复元素前的元素都要清除掉。

不断重复上面2步，直到i或j到达字符串末尾。

时间复杂度o(n)
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max(a,b) ((a)>(b)?(a):(b))

int lengthOfLongestSubstring(char *s, int len) {
    int ans = 0;
    int index[128]; // current index of character
    // try to extend the range [i, j]
    for (int j = 0, i = 0; j < len; j++) {
        i = max(index[s[j]], i);//慢指针移动到字符上次出现的位置
        ans = max(ans, j - i + 1);
        index[s[j]] = j + 1;//记录字符本次出现的位置
		printf("i=%d,j=%d,ans=%d\n",i,j,ans);
        printf("index['a']=%d,index['b']=%d,index['c']=%d\n",
              index['a'],index['b'],index['c']);
    }
    return ans; 
}

int main()
{
    char *str = "bbcac";
    int name2 = lengthOfLongestSubstring(str, strlen(str));
    printf("%d\n",name2);
    return 0;
}