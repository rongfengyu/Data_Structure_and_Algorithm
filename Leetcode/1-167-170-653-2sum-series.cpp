/*
1-167-170-653 题型相似，只是内部条件有所不同，解题思路一致，使用排序 + 双指针，时间复杂度O(N^2)

1. 两数之和    two-sum
给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那 两个 整数，并返回他们的数组下标。

你可以假设每种输入只会对应一个答案。但是，数组中同一个元素不能使用两遍。

示例:

给定 nums = [2, 7, 11, 15], target = 9

因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        
    }
};

167. 两数之和 II - 输入有序数组    two-sum-ii-input-array-is-sorted
给定一个已按照升序排列 的有序数组，找到两个数使得它们相加之和等于目标数。

函数应该返回这两个下标值 index1 和 index2，其中 index1 必须小于 index2。

说明:

返回的下标值（index1 和 index2）不是从零开始的。
你可以假设每个输入只对应唯一的答案，而且你不可以重复使用相同的元素。
示例:

输入: numbers = [2, 7, 11, 15], target = 9
输出: [1,2]
解释: 2 与 7 之和等于目标数 9 。因此 index1 = 1, index2 = 2 。


class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {

    }
};



170. 两数之和 III - 数据结构设计
设计并实现一个 TwoSum 的类，使该类需要支持 add 和 find 的操作。

add 操作 - 对内部数据结构增加一个数。
find 操作 - 寻找内部数据结构中是否存在一对整数，使得两数之和与给定的数相等。
示例 1:

add(1); add(3); add(5);
find(4) -> true
find(7) -> false
示例 2:

add(3); add(1); add(2);
find(3) -> true
find(6) -> false


我的思路：直接使用vector<int>，支持add - push_back，find - 遍历，排序后第一题


653. 两数之和 IV - 输入 BST    two-sum-iv-input-is-a-bst
给定一个二叉搜索树和一个目标结果，如果 BST 中存在两个元素且它们的和等于给定的目标结果，则返回 true。

案例 1:

输入: 
    5
   / \
  3   6
 / \   \
2   4   7

Target = 9

输出: True
 

案例 2:

输入: 
    5
   / \
  3   6
 / \   \
2   4   7

Target = 28

输出: False

//
// * Definition for a binary tree node.
// * struct TreeNode {
// *     int val;
// *     TreeNode *left;
// *     TreeNode *right;
// *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
// * };
//
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {

    }
};


解题思路：

1、2sum系列
可以使用暴力法，使用两层循环，时间复杂度为O(N^2)

2、显而易见的暴力求解方法，使用三重循环，时间复杂度为O(N^3)

3、一般解题思路：先对数组进行排序，然后固定一个值，使用双指针夹逼移动进行求解，注意细节，时间复杂度为O(N^2)

*/


//leetcode-1
//方法一：暴力法，两层循环，时间复杂度为O(N^2)

//方法二：【题目标签-哈希表】使用C++ STL map，时间复杂度为O(N)，循环遍历元素Ai，并且判断是否target-Ai已存在，已存在则返回，不存在则添加到map中。
//此方法不同于先把所有元素都存入map，可避免2+2=4的情况出现(题目已说明返回唯一)

//方法三：手撕hash表，冲突解决方法是-拉链法


//leetcode167
//方法一：暴力法，两层循环，时间复杂度为O(N^2)，并未使用“输入有序数组”这一条件！！！

//方法二：单层循环 + （剩下部分）二分查找，时间复杂度为O(N*logN)

//方法三：双指针法（或 对撞指针法 或 双索引法），时间复杂度为O(N)
//【充分利用输入有序的条件】首先判断首尾两项的和是不是target，如果比target小，那么我们左边+1位置的数（比左边位置的数大）再和右相相加，继续判断。
//如果比target大，那么我们右边-1位置的数（比右边位置的数小）再和左相相加，继续判断。
//我们通过这样不断放缩的过程，就可以在O(n)的时间复杂度内找到对应的坐标位置。（这和快速排序的思路很相似）


//leetcode653
//BST中序遍历是有序数组 + leetcode167
	

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//leetcode167
vector<int> twoSum167(vector<int>& numbers, int target) 
{
	vector<int> result;
	sort(numbers.begin(),numbers.end());
	
	int l = 0;
	int r = numbers.size() - 1;
	
	while(l < r)
	{
		if(numbers[l] + numbers[r] > target)
			r -= 1;
		else if(numbers[l] + numbers[r] < target)
			l += 1;
		else
		{
			result.push_back(l+1);//题目要求索引从1开始
			result.push_back(r+1);
			return result;
		}
	}

	result.push_back(0);//题目要求索引从1开始
	result.push_back(0);
	return result;
}

int main() {
	
	vector<int> tmp;
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(3);
	tmp.push_back(4);
	tmp.push_back(5);
	tmp.push_back(6);
	
	vector<int> answer;
	
	answer = twoSum167(tmp, 3);
	cout << "answer = " << answer.size() << endl;
	
	int j = 0;
	for(j=0; j<answer.size(); j++)
	{
		cout<<answer[j]<<" ";
	}
	cout<<endl;
	
	return 0;
}



















