/*
面试题05. 替换空格
请实现一个函数，把字符串 s 中的每个空格替换成"%20"。

 

示例 1：

输入：s = "We are happy."
输出："We%20are%20happy."
 

限制：

0 <= s 的长度 <= 10000
*/

//思路一：开辟len*3的空间（最坏情况），遍历原字符串，遇到空格则替换为“%20”
class Solution {
    public String replaceSpace(String s) {
        int length = s.length();
        char[] array = new char[length * 3];
        int size = 0;
        for (int i = 0; i < length; i++) {
            char c = s.charAt(i);
            if (c == ' ') {
                array[size++] = '%';
                array[size++] = '2';
                array[size++] = '0';
            } else {
                array[size++] = c;
            }
        }
        String newStr = new String(array, 0, size);
        return newStr;
    }
}
//时间复杂度：O(n)。遍历字符串 s 一遍。
//空间复杂度：O(n)。额外创建字符数组，长度为 s 的长度的 3 倍。

//思路二：原位扩充，从后向前。遍历字符串，统计空格数，按计数增加字符串长度，然后从后向前替换，可利用原位空间
class Solution {
public:
    string replaceSpace(string s) {
        int count = 0, length = s.length();

        for (int i = 0; i < length; i++) {
            if (s[i] == ' ') {
                count++;
            }
        }

        if (count == 0) {
            return s;
        }

        for (int i = 0; i < count; i++) {
            s += "00";
        }

        int new_length = s.length();

        int j = new_length - 1;

        for (int i = length - 1; i >= 0; i--) {
            if (s[i] != ' ') {
                s[j--] = s[i];
            } else {
                s[j--] = '0';
                s[j--] = '2';
                s[j--] = '%';
            }
        }

        return s;
    }
};
//时间复杂度：O(n)。遍历字符串 s 一遍。
//空间复杂度：O(1)。可以认为是常数空间