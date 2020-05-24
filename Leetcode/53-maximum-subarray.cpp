/*
53. 最大子序和
给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

示例:

输入: [-2,1,-3,4,-1,2,1,-5,4],
输出: 6
解释: 连续子数组 [4,-1,2,1] 的和最大，为 6。
进阶:

如果你已经实现复杂度为 O(n) 的解法，尝试使用更为精妙的分治法求解。

*/

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<memory.h>
#define k 0x3f3f3f3f
using namespace std;

extern int ii;

int main(){
	int ii=100;
    int n;
    long long int temp;
    long long int sum=0;
    long long int Max = -k;
	
	int a[] = {-2,1,-3,4,-1,2,1,-5,4};
	
	sum = 0;
	Max = -k;
	for(int i=0; i<sizeof(a)/sizeof(int); i++){
		temp = a[i];
		sum = max(sum+temp, temp);
		cout << "sum = " << sum <<"  ";
		Max = max(Max, sum);
		cout << "Max = " << Max <<endl;
	}
	cout<<Max<<endl;
	
	cout<<ii<<endl;
}

//O(N),O(1)

class Solution {
    // 动态规划
    public int maxSubArray(int[] nums) {
        if (nums == null || nums.length == 0) return 0;
        int ans = 0;

        // 1. 状态定义
        // dp[i] 表示前 i 个元素的最大连续子数组的和
        int[] dp = new int[nums.length];

        // 2. 状态初始化，数组中第一个元素的最大和就是第一个元素值
        dp[0] = nums[0];
        ans = nums[0];

        // 3. 状态转移  dp[i]定义为以nums[i]为结尾的“最大子数组和”
        // 转移方程：dp[i] = max(dp[i - 1], 0) + nums[i]
        //  dp 当前元素的值等于前一个元素值和 0 的最大值再加上 nums[i]
        for (int i = 1; i < nums.length; i++) {
            dp[i] = Math.max(dp[i - 1], 0) + nums[i];
            // 更新最大和
            ans = Math.max(ans, dp[i]);
        }

        return ans;
    }
}

//O(N),O(N)

//状态转移方程 dp[i] = max(dp[i - 1], 0) + nums[i] 看出，当前的状态的值只取决于前一个状态值，所以我们可以使用一个变量来代替 dp[i] 和 dp[i - 1]

class Solution {
    // 动态规划
    public int maxSubArray(int[] nums) {
        if (nums == null || nums.length == 0) return 0;
        int ans = 0;

        // 使用 currSum 代替 dp[i]
        int currSum = nums[0];
        ans = nums[0];

        for (int i = 1; i < nums.length; i++) {
            currSum = Math.max(currSum, 0) + nums[i];
            // 更新最大和
            ans = Math.max(ans, currSum);
        }

        return ans;
    }
}

//O(N),O(1)




//分治法，二分递归，难以理解
//O(NlogN),O(logN)递归时栈使用的空间
class Solution
{
public:
    int maxSubArray(vector<int> &nums)
    {
        //类似寻找最大最小值的题目，初始值一定要定义成理论上的最小最大值
        int result = INT_MIN;
        int numsSize = int(nums.size());
        result = maxSubArrayHelper(nums, 0, numsSize - 1);
        return result;
    }

    int maxSubArrayHelper(vector<int> &nums, int left, int right)
    {
        if (left == right)
        {
            return nums[left];
        }
        int mid = (left + right) / 2;
        int leftSum = maxSubArrayHelper(nums, left, mid);
        //注意这里应是mid + 1，否则left + 1 = right时，会无线循环
        int rightSum = maxSubArrayHelper(nums, mid + 1, right);
        int midSum = findMaxCrossingSubarray(nums, left, mid, right);
        int result = max(leftSum, rightSum);
        result = max(result, midSum);
        return result;
    }

    int findMaxCrossingSubarray(vector<int> &nums, int left, int mid, int right)
    {
        int leftSum = INT_MIN;
        int sum = 0;
        for (int i = mid; i >= left; i--)
        {
            sum += nums[i];
            leftSum = max(leftSum, sum);
        }

        int rightSum = INT_MIN;
        sum = 0;
        //注意这里i = mid + 1，避免重复用到nums[i]
        for (int i = mid + 1; i <= right; i++)
        {
            sum += nums[i];
            rightSum = max(rightSum, sum);
        }
        return (leftSum + rightSum);
    }
};
