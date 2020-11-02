/*
446. 等差数列划分 II - 子序列
如果一个数列至少有三个元素，并且任意两个相邻元素之差相同，则称该数列为等差数列。

例如，以下数列为等差数列:

1, 3, 5, 7, 9
7, 7, 7, 7
3, -1, -5, -9
以下数列不是等差数列。

1, 1, 2, 5, 7
 

数组 A 包含 N 个数，且索引从 0 开始。该数组子序列将划分为整数序列 (P0, P1, ..., Pk)，满足 0 ≤ P0 < P1 < ... < Pk < N。

如果序列 A[P0]，A[P1]，...，A[Pk-1]，A[Pk] 是等差的，那么数组 A 的子序列 (P0，P1，…，PK) 称为等差序列。值得注意的是，这意味着 k ≥ 2。

函数要返回数组 A 中所有等差子序列的个数。

输入包含 N 个整数。每个整数都在 -2^31 和 2^31-1 之间，另外 0 ≤ N ≤ 1000。保证输出小于 2^31-1。


示例：

输入：[2, 4, 6, 8, 10]

输出：7

解释：
所有的等差子序列为：
[2,4,6]
[4,6,8]
[6,8,10]
[2,4,6,8]
[4,6,8,10]
[2,4,6,8,10]
[2,6,10]


直接枚举搜索的复杂度是O(n!)的。

动态规划
定义： dp[i][d]表示以A[i]结尾,等差为d，数列长度大于1的等差数列的个数。

定义弱等差数列： 
弱等差数列 是至少有两个元素的子序列，任意两个连续元素的差相等。
 
f[i][d]代表以 A[i] 结束且公差为 d 的弱等差数列个数。
 
现在状态转移方程就十分简单：
 
对于所有 j < i，f[i][A[i] - A[j]] += (f[j][A[i] - A[j]] + 1)。
 
这里的 1 是对 (i, j)生成一个新的弱等差数列。
 
对于 f[i][A[i] - A[j]] += (f[j][A[i] - A[j]] + 1)，f[j][A[i] - A[j]] 是现有的弱等差数列个数，而 1 是根据 A[i] 和 A[j] 新建的子序列。新增加的序列必为等差数列。故 f[j][A[i] - A[j]] 为新生成的等差数列的个数。

*/

#include <iostream>
#include <map>
#include <vector>

using namespace std;


#define LL long long
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        int n = A.size();
        LL ans = 0;
        vector<map<LL, int> > cnt(n);
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                LL delta = (LL)A[i] - (LL)A[j];
                int sum = 0;
                if (cnt[j].find(delta) != cnt[j].end()) {//cnt[j][delta]已存在,保证数列长度大于2
                    sum = cnt[j][delta];
					cout << "-" << j << "|" << delta << "|" <<cnt[j][delta] << endl;
					cout << "sum=" << sum  << endl;
                }
                cnt[i][delta] += sum + 1;//自加是因为会存在相同元素，相同元素需要重复计数
				cout << i << "|" << delta << "|" << cnt[i][delta] << endl;
                ans += sum;
            }
        }

        return (int)ans;
    }
};


int main() {
	int a[5] = {1,2,3,4,5};
	vector<int> b(a, a+5);
	
	Solution solution;
	
	int ans = solution.numberOfArithmeticSlices(b);
	
	cout << "ans=" << ans << endl;
	return 0;
}

1|1|1
2|2|1
-1|1|1
sum=1  1,2,3
2|1|2
3|3|1
3|2|1
-2|1|2
sum=2 2,3,4 1,2,3,4
3|1|3
4|4|1
4|3|1
-2|2|1
sum=1 1,3,5
4|2|2
-3|1|3
sum=3 3,4,5 2,3,4,5 1,2,3,4,5
4|1|4
ans=7