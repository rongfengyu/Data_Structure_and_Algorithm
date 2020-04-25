/*
45. 跳跃游戏 II
给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

你的目标是使用最少的跳跃次数到达数组的最后一个位置。

示例:

输入: [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
说明:

假设你总是可以到达数组的最后一个位置。


注解：

【很重要的说明】：总是可以到达数组的最后一个位置！！！  可以排除leetcode55有坑的那种情况
如{2,1,1,3,2,1,0,1}，此时终点不可到达

1、每个元素是可以跳跃的最大长度，而不是必须跳跃的长度；

2、需要知道上次能到达的位置，作为这一次的起点。然后需要知道此次能到达的最远的位置，作为下一次的起点。

3、贪心算法：我们在跳的时候会考虑到下一步。根据下一步能跳的最远的贪心策略，来进行每一步的选择。
扫描数组，以确定当前最远能覆盖的节点。然后继续扫描，直到当前的路程超过了上一次算出的覆盖范围，那么更新覆盖范围，同时更新条数，因为我们是经过了多一跳才能继续前进的。
形象地说，这个是在争取每跳最远的greedy。

*/

#include <stdio.h>
	
int jump(int* nums, int numsSize)
{
    int cnt = 0;
    if (nums == NULL || numsSize == 0) {
        return cnt;
    }
    
    int maxPos = 0; //下次最远能到达的位置
    int right = 0;   //本次次最远能到达的位置
    
    for (int i = 0; i < numsSize - 1; i++) {
        maxPos = (nums[i] + i) > maxPos ? (nums[i] + i) : maxPos;//使用三元操作符来操作变量的变化，是一个很高端的用法
        printf("i=%d, maxPos = %d\n", i, maxPos);

        if (i == right) {
            cnt++;//存在计数滞后

            right = maxPos;
            printf("right = %d\n", right);
        }

    }
    return cnt;
}

int main(int argc, char* argv[])
{
    int nums[] = {2,3,1,1,2,2,1,1};
    int size = sizeof(nums)/sizeof(int);

    int ret = jump(nums, size);
	
	printf("ret = %d ", ret);

    return 0;
}


【扩展】
测试用例，如果避不开0这个坑，应该是无解的！
{2,1,1,3,2,1,0,1};

#include <stdio.h>
#include <vector>
using namespace std;
class Solution{
public:
	int jumpGame2(vector<int> steps){
		//如果格子小于等于1，返回错误1，一步都不跳
		if(steps.size() <= 1)
			return 0;
		//current_index = steps[a]+a;
		int current_index = steps[0];
		//pre_index = (a,b] 之间的能去到的最远距离
		int pre_index = steps[0];
		//跳跃次数
		int times = 0;
		//记录每次跳哪一步
		int theBestChoise = 0;
		for(int i = 1;i < steps.size();i++){
			//指针来到 b+1的位置，不得不跳出一步
			if(i > current_index){
				current_index = pre_index;
				//不得不走一步，只能走到一个坑里面时，结束了
				if(steps[current_index] == 0)
                    return 600;
				printf("跳跃到第%d个格子\n",theBestChoise);
				times++;
			}
			//看pre_index 能否被超过
			if(pre_index < i+steps[i]){
				theBestChoise = i;
				pre_index = i+steps[i];
                if(pre_index >= steps.size()-1){ //能够到终点了，直接退出
                    times = times + 2; //这里连续跳了两步
                    printf("跳跃到第%d个格子,然后直接跳到终点\n",theBestChoise);
                    break;
                }
			}
		}
		return times;
	}
};
 
int main(){
	vector<int> steps;
	steps.push_back(1);
	steps.push_back(1);
	steps.push_back(3);
	steps.push_back(2);
	steps.push_back(1);
	//steps.push_back(0);  //注释这一行看这个坑有没有起效
	steps.push_back(2);
	steps.push_back(3);
	steps.push_back(5);
	steps.push_back(2);
	Solution solve;
	int result = solve.jumpGame2(steps);
	if(result == 0)
        printf("你只有一个格子或者一个格子都没有，跳0步！");
    else if(result == 600)
        printf("你不得不跳入一个坑中，结束一生！");
    else
        printf("总共需要跳跃%d步！",result);
}