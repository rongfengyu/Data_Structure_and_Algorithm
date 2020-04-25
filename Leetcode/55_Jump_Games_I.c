/*
55. 跳跃游戏
给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个位置。

示例 1:

输入: [2,3,1,1,4]
输出: true
解释: 我们可以先跳 1 步，从位置 0 到达 位置 1, 然后再从位置 1 跳 3 步到达最后一个位置。
示例 2:

输入: [3,2,1,0,4]
输出: false
解释: 无论怎样，你总会到达索引为 3 的位置。但该位置的最大跳跃长度是 0 ， 所以你永远不可能到达最后一个位置。


注解：
对比leetcode45，相似但是不同，相比来说，更加容易判断是否可以到达，依然使用贪婪算法，遍历数组，判断覆盖范围是否可以到达结尾即可

1、每个元素是可以跳跃的最大长度，而不是必须跳跃的长度；

2、需要知道上次能到达的位置，作为这一次的起点。然后需要知道此次能到达的最远的位置，作为下一次的起点。

3、贪心算法：遍历数组，判断覆盖范围是否可以到达结尾即可（类似感染传播的一种思想）

*/

#include <stdio.h>
#include <vector>
using namespace std;

class Solution {
public:
    bool canJump(vector<int>& nums) {
        // rightMost is the farthest index we can reach
        int rightMost = 0;
        for (int i = 0; i < nums.size() - 1 && rightMost < nums.size(); ++i) {
            if (rightMost < i) break;
            rightMost = max(rightMost, i + nums[i]);
        }
        return rightMost >= nums.size() - 1;
    }
};

 
int main(){
	vector<int> steps;
	steps.push_back(3);
	steps.push_back(2);
	steps.push_back(1);
	steps.push_back(0);  //注释这一行看这个坑有没有起效
	steps.push_back(4);
	Solution solve;
	bool result = solve.canJump(steps);
	if(result)
        	printf("true\n");
    	else
        	printf("false\n");
}
