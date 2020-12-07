/*
514、自由之路

题目描述
在电视游戏 Fallout 4 中，“通向自由之路”的请求需要玩家在一个金属拨号器通话，并使用拨号器按顺序拼出一个特殊的单词以打开自由之门。

给定一个 环形字符串，代表刻在拨号器上的代码，和另一个 特殊字符串，代表需要被拼出的单词。你需要使用 最少 的步数拨动拨号器按顺序拼出特定单词的所有字母。

初始时，环形字符串 的第一个字母在 12:00 方向，你需要通过顺时针或逆时针拨动环形拨号器使某个字母指向 12:00 方向，来一个接一个地拼出 特殊单词 的所有字母，在找到对应字母后，按动中间的按钮完成。

在转动拨号器拼写特殊字母 key[i] 的阶段：
1. 你可以每次顺时针或逆时针转动 1 次，算 1 步操作，目的是使环形字符串的某个字母指向 12:00 方向，且该字母必须等于 key[i]。
2. 如果字母 key[i] 已经在 12:00 方向，你需要按动中间的按钮拼写，这也将计算 1 步。按下之后，你才可以拼写下一个字母 （下一阶段的操作）。否则，你必须完成当前字母的拼写。

作者：wzc1995
链接：https://www.acwing.com/solution/leetcode/content/410/
来源：AcWing
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
*/

class Solution {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();
        vector<vector<int>> dp(m + 1, vector<int>(n));
        for (int i = m - 1; i >= 0; --i) {
            for (int j = 0; j < n; ++j) {
                dp[i][j] = INT_MAX;
                for (int k = 0; k < n; ++k) {
                    if (ring[k] == key[i]) {
                        int diff = abs(j - k);
                        int step = min(diff, n - diff);
                        dp[i][j] = min(dp[i][j], step + dp[i + 1][k]);
                    }
                }
            }
        }
        return dp[0][0] + m;      
    }
};