/*
41. 缺失的第一个正数
给你一个未排序的整数数组，请你找出其中没有出现的最小的正整数。

 

示例 1:

输入: [1,2,0]
输出: 3
示例 2:

输入: [3,4,-1,1]
输出: 2
示例 3:

输入: [7,8,9,11,12]
输出: 1
 

提示：

你的算法的时间复杂度应为O(n)，并且只能使用常数级别的额外空间。


注解：

利用符号位做标记，以及利用数组索引


常规思路就是排序，扫一遍。时间复杂度为 O(nlogn) 。

题干要求的时间复杂度让我们想到需要类似于计数排序，但计数排序的空间复杂度并不满足题干要求。

挖掘题面信息，我们发现题目要求的是正整数，int型最高位为0。因此我们可以利用好这个最高位来打标记。

我们首先可以预处理出所有正整数，然后用类似计数排序去标记该数组中的正整数，答案最大为数组中的正整数的个数 + 1，若遇到更大的正整数，直接忽略掉不做标记即可。

时间复杂度为 O(n) ，额外空间复杂度为 O(1) 。

如2, 5, -1, 1, 预处理后为2, 5, 1, 各正数为02, 05, 01, (0表示符号位), 扫到2时, 数组变为02, 15, 01; 

扫到5时，越界，忽略, 数组仍为02, 15, 01; 扫到1时, 数组为12, 15, 01。

最后计算答案时，从前往后扫描，发现第3个数符号位为0, 故答案为3。


*/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
 
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int i = 0, j = 0;
        for( ; i < nums.size(); i++)        //预处理，nums[0 ~ j-1]为所有的j个正整数
            if(nums[i] > 0) nums[j++] = nums[i];
        for(int i = 0; i < j; i++){
            int n = nums[i]&0x7fffffff;        //取出所存的正整数
            if(n <= j) nums[n-1] |= 0x80000000;//标记该正整数的存在
        }
        int ans = 0;    //遍历查找第一个不存在的正整数
        while(ans < j && (nums[ans]&0x80000000)) ans++;
        return ans+1;
    }
};

int main()
{
	Solution solution;
	vector<int> A;
	A.push_back(2);
	A.push_back(3);
	A.push_back(6);
	A.push_back(7);
	
    int result = 0;
    result = solution.firstMissingPositive(A);

    cout << "result = " << result << endl;	
 
    return 0;
}
