/*
650. 只有两个键的键盘
最初在一个记事本上只有一个字符 'A'。你每次可以对这个记事本进行两种操作：

Copy All (复制全部) : 你可以复制这个记事本中的所有字符(部分的复制是不允许的)。
Paste (粘贴) : 你可以粘贴你上一次复制的字符。
给定一个数字 n 。你需要使用最少的操作次数，在记事本中打印出恰好 n 个 'A'。输出能够打印出 n 个 'A' 的最少操作次数。

示例 1:

输入: 3
输出: 3
解释:
最初, 我们只有一个字符 'A'。
第 1 步, 我们使用 Copy All 操作。
第 2 步, 我们使用 Paste 操作来获得 'AA'。
第 3 步, 我们使用 Paste 操作来获得 'AAA'。
说明:

n 的取值范围是 [1, 1000] 。
*/


//动态规划

class Solution {
public:
    int minSteps(int n) {
        vector<vector<long>> dp(n+1,vector<long>(n+1,INT_MAX));
        dp[1][0] = 0;
        dp[1][1] = 1;
        for(int i = 1;i<n+1;i++){
            long minStep = dp[i][0];
            for(int j = 1;j<=i;j++){
                if(i>j){
                    dp[i][j] = min(dp[i-j][j]+1,dp[i][j]);
                    minStep = min(dp[i][j],minStep);
                }
                if(i==j){
                    dp[i][j] = minStep+1;
                }
            }
        }
        return dp[n][n]-1;
    }
};

class Solution {
public:
    int minSteps(int n) {
        vector<int> dp(n+1,0);
        int i, j;
        for(i = 2; i <= n ; ++i)
        {
        	dp[i] = i;	//一直粘贴的次数
        	for(j = 2; j < i; ++j)
        	{
        		if(i%j == 0)
        			dp[i] = min(dp[i], dp[j]+i/j);
        	}
        }
        return dp[n];
    }
};



//数学法 - 因数分解

/*
将所有操作分成以 copy 为首的多组，形如 (copy, paste, ..., paste)，再使用 C 代表 copy，P 代表 paste。例如操作 CPPCPPPPCP 可以分为 [CPP][CPPPP][CP] 三组。

假设每组的长度为 g_1, g_2, ...。完成第一组操作后，字符串有 g_1 个 A，完成第二组操作后字符串有 g_1 * g_2 个 A。当完成所有操作时，共有 g_1 * g_2 * ... * g_n 个 'A'。

我们最终想要 N = g_1 * g_2 * ... * g_n 个 A。如果 g_i 是合数，存在 g_i = p * q，那么这组操作可以分解为两组，第一组包含 1 个 C 和 p-1 个 P，第二组包含 1 个 C 和 q-1 个 P。

现在证明这种分割方式使用的操作最少。原本需要 pq 步操作，分解后需要 p+q 步。因为 p+q <= pq，等价于 1 <= (p-1)(q-1)，当 p >= 2 且 q >= 2 时上式永远成立。

算法

假设 g_1, g_2, ... 就是 N 的素数分解，则需要的最少操作等于这些素数之和。


复杂度分析

时间复杂度：O(sqrt{N})，当 N 是素数的平方时，需要循环 O(sqrt{N})步。

空间复杂度：O(1)，ans 和 d 的存储空间。
*/

class Solution(object):
    def minSteps(self, n):
        ans = 0
        d = 2
        while n > 1:
            while n % d == 0:
                ans += d
                n /= d
            d += 1
        return ans

