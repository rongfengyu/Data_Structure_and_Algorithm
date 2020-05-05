/*
39. 组合总和
给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。

candidates 中的数字可以无限制重复被选取。

说明：

所有数字（包括 target）都是正整数。
解集不能包含重复的组合。 
示例 1:

输入: candidates = [2,3,6,7], target = 7,
所求解集为:
[
  [7],
  [2,2,3]
]
示例 2:

输入: candidates = [2,3,5], target = 8,
所求解集为:
[
  [2,2,2,2],
  [2,3,3],
  [3,5]
]


注解：

回溯算法，剪枝


*/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
 
class Solution {
public:
    vector<vector<int> > combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        result.clear();
        helper(candidates, vector<int>(), 0, target);
        return result;
    }
private:
    void helper(vector<int>& candidates, vector<int> cur, int start, int target){
        if(target==0) {//结果判定
            result.push_back(cur);
            return;
        }
        for(int i=start; i<candidates.size(); i++){
            if(target<candidates[i]) break;//剪枝
			
            cur.push_back(candidates[i]);
            helper(candidates, cur, i, target-candidates[i]);//修改i，可以控制是否可以使用重复值
            cur.pop_back();
        }
    }
    vector<vector<int> > result;
};

void printfVector(vector<int> vec)
{
	int i = 0;
	for( i = 0; i < vec.size(); i++ )
   {
    cout << vec[i] << " ";
   }
	cout <<endl;
}

int main()
{
	Solution solution;
	vector<int> A;
	A.push_back(2);
	A.push_back(3);
	A.push_back(6);
	A.push_back(7);
	
	vector<vector<int> > result;
	result = solution.combinationSum(A,6);
	
	int i = 0;
	for( i = 0; i < result.size(); i++ )
   {
		printfVector(result[i]);
   }
 
    return 0;
}
