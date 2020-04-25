/*
32. 最长有效括号
给定一个只包含 '(' 和 ')' 的字符串，找出最长的包含有效括号的子串的长度。

示例 1:

输入: "(()"
输出: 2
解释: 最长有效括号子串为 "()"
示例 2:

输入: ")()())"
输出: 4
解释: 最长有效括号子串为 "()()"

题目解析


1、【重点】使用栈的思路


这个题目的标签是 栈 ，我们考虑一下怎么借助 栈 来解决。

不过这个栈有点特殊，它是 递减栈 ：栈里只有递减元素。

具体操作如下：

遍历整个数组，如果栈不空，且当前数字大于栈顶元素，那么如果直接入栈的话就不是 递减栈 ，所以需要取出栈顶元素，

由于当前数字大于栈顶元素的数字，而且一定是第一个大于栈顶元素的数，直接求出下标差就是二者的距离。

继续看新的栈顶元素，直到当前数字小于等于栈顶元素停止，然后将数字入栈，这样就可以一直保持递减栈，且每个数字和第一个大于它的数的距离也可以算出来。

由于传入的参数是数组，入栈的可以是数组索引，而不是该数元素！

2、动态规划-序列型动态规划

可以定义 dp[i] 表示的是 str[0…i] 的答案，思路其实和前面很类似：

1、从左到右遍历输入的字符串

2、如果遇到的是 '('，意味着这并不能和前面遍历过的部分组成合法答案，因为 dp 状态数组中记录的是答案，这个时候说明 dp[i] = 0，也就是不用做任何记录

3、如果遇到的是 ')'，这时我们还是需要往前看：

如果 str[i - 1] 是 '('，那么 dp[i] = dp[i - 2] + 2
如果 str[i - 1] 是 ')'，这表示 str[i - 1] 已经配对了，因此我们还要继续往前看，从当前位置往左，看第一个没有被配对的 '('，怎么找这个位置呢，这里我们就可以利用 dp[i - 1] 这个信息，dp[i - 1] 表示的是之前匹配的长度，那么 ：
i - dp[i - 1] - 1 表示的就是从当前位置往左，第一个没有被配对的位置
如果位置 i 和 位置 i - dp[i - 1] - 1 配对后，我们可以看看当前的序列是否可以和之前匹配的序列链接起来，也就是加上 dp[i - dp[i - 1] - 2]

3、借助变量
使用了两个变量 Left 和 Right，分别用来记录到当前位置时左括号和右括号的出现次数。

当遇到左括号时，Left 自增 1，右括号时 Right 自增1。

对于最长有效的括号的子串，一定是左括号等于右括号的情况，此时就可以更新结果 res 了，一旦右括号数量超过左括号数量了，说明当前位置不能组成合法括号子串，Left 和 Right 重置为 0。

但是对于这种情况 "(()" 时，在遍历结束时左右子括号数都不相等，此时没法更新结果 res，但其实正确答案是 2，怎么处理这种情况呢？

答案是再 反向遍历一遍 ，采取类似的机制，稍有不同的是此时若 Left 大于 Right 了，则重置 0，这样就可以涵盖所有情况。

https://mp.weixin.qq.com/s/0HLtrZbsZvUR_FJIAizjfw

*/


#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        
    if (s.size() == 0) {
        return 0;
    }

    stack<int> st;
    int result = 0;//记录结果长度

    for (int i = 0; i < s.size(); ++i) {		
		
        // stack.size() > 1 表示栈不为空，而且我们必须保证栈顶元素是 '('
        if (s[i] == ')' && (!st.empty()) && s[st.top()] == '(') {
            
            // 记录长度
            result = max(result, i - st.top() + 1);//由于我们没有设置栈低的保护元素（一般为-1），所以要在记录长度后再出栈（空栈top会段错误），并且长度要加一
			
	    // 配对的 '(' 出栈
            st.pop();
			
        } else { // 其他情况，直接将当前位置入栈
			
            st.push(i);
        }
		
	cout<< "em = " << st.empty() <<endl;
	cout<< "size = " << st.size() <<endl;
    }
    return result;
    }
};

int main(){
	string s = "()()())";
	
	Solution solve;
	int result = solve.longestValidParentheses(s);

    cout<< "result = " << result <<endl;
}


//该方法只需要对数组进行一次遍历，每个元素最多被压入和弹出堆栈一次，算法复杂度是 O(n)。


//2、动态规划-序列型动态规划  空间复杂度O(n)
public int longestValidParentheses(String s) {
    if (s == null || s.length() == 0) {
        return 0;
    }

    int n = s.length();

    char[] sArr = s.toCharArray();

    int[] dp = new int[n];

    int result = 0;
    for (int i = 1; i < n; ++i) {
        if (sArr[i] == ')') {
            // 如果前一个位置是 '('，直接配对
            if (sArr[i - 1] == '(') {
                dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
            } 
            // 前一个位置是 ')'
            // 我们从当前位置往左看，如果第一个没有被匹配的位置是 '('
            // 表明当前位置是可以被匹配的
            else if (i - dp[i - 1] - 1 >= 0 && sArr[i - dp[i - 1] - 1] == '(') {
                // 这里其实是 dp[i] = i - (i - dp[i - 1] - 1) + 1 = dp[i - 1] + 2
                // 但是我们还需要考虑之前的答案，也就是 dp[i - dp[i - 1] - 2]
                // 首先判断 i - dp[i - 1] - 2 是否越界
                // 如果没有越界就将其加上
                dp[i] = dp[i - 1] + 2;

                if (i - dp[i - 1] >= 2) {
                    dp[i] += dp[i - dp[i - 1] - 2];
                }
            }

            result = Math.max(result, dp[i]);
        }
    }

    return result;
}


//3、借助变量

时间复杂度O(2N)，空间复杂度O(1)

//代码来源：https://leetcode-cn.com/problems/longest-valid-parentheses/solution/zui-chang-you-xiao-gua-hao-by-leetcode/

public class Solution {
    public int longestValidParentheses(String s) {
        int left = 0, right = 0, maxlength = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == '(') {
                left++;
            } else {
                right++;
            }
            if (left == right) {
                maxlength = Math.max(maxlength, 2 * right);
            } else if (right > left) {
                left = right = 0;
            }
        }
        left = right = 0;
        for (int i = s.length() - 1; i >= 0; i--) {
            if (s.charAt(i) == '(') {
                left++;
            } else {
                right++;
            }
            if (left == right) {
                maxlength = Math.max(maxlength, 2 * left);
            } else if (left > right) {
                left = right = 0;
            }
        }
        return maxlength;
    }
}
















