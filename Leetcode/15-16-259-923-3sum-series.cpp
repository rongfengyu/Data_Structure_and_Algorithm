/*
15-16-259-923 题型相似，只是内部条件有所不同，解题思路一致，使用双指针，时间复杂度O(N^2)

15. 三数之和
给你一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有满足条件且不重复的三元组。

注意：答案中不可以包含重复的三元组。

示例：

给定数组 nums = [-1, 0, 1, 2, -1, -4]，

满足要求的三元组集合为：
[
  [-1, 0, 1],
  [-1, -1, 2]
]


// * Return an array of arrays of size *returnSize.
// * The sizes of the arrays are returned as *returnColumnSizes array.
// * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes){

}

16. 最接近的三数之和
给定一个包括 n 个整数的数组 nums 和 一个目标值 target。找出 nums 中的三个整数，使得它们的和与 target 最接近。返回这三个数的和。假定每组输入只存在唯一答案。

例如，给定数组 nums = [-1，2，1，-4], 和 target = 1.

与 target 最接近的三个数的和为 2. (-1 + 2 + 1 = 2).

int threeSumClosest(int* nums, int numsSize, int target){

}

259. 较小的三数之和
Given an array of n integers nums and a target, find the number of index triplets i, j, k with 0 <= i < j < k < n that satisfy the condition nums[i] + nums[j] + nums[k] < target.

For example, given nums = [-2, 0, 1, 3], and target = 2.

Return 2. Because there are two triplets which sums are less than 2:

[-2, 0, 1]
[-2, 0, 3]
===============
给定n个整数nums和一个目标的数组，找出满足条件nums[i]+nums[j]+nums[k]<目标的索引三元组i、j、k的个数，其中0<=i<j<k<n。

例如，给定nums=[-2，0，1，3]和target=2。

返回2。因为有两个三元组的和小于2：

[-2, 0, 1]
[-2, 0, 3]

int threeSumSmaller(int* A, int ASize, int target){

}


923. 三数之和的多种可能
给定一个整数数组 A，以及一个整数 target 作为目标值，返回满足 i < j < k 且 A[i] + A[j] + A[k] == target 的元组 i, j, k 的数量。

由于结果会非常大，请返回 结果除以 10^9 + 7 的余数。
 

示例 1：

输入：A = [1,1,2,2,3,3,4,4,5,5], target = 8
输出：20
解释：
按值枚举（A[i]，A[j]，A[k]）：
(1, 2, 5) 出现 8 次；
(1, 3, 4) 出现 8 次；
(2, 2, 4) 出现 2 次；
(2, 3, 3) 出现 2 次。
示例 2：

输入：A = [1,1,2,2,2,2], target = 5
输出：12
解释：
A[i] = 1，A[j] = A[k] = 2 出现 12 次：
我们从 [1,1] 中选择一个 1，有 2 种情况，
从 [2,2,2,2] 中选出两个 2，有 6 种情况。
 

提示：

3 <= A.length <= 3000
0 <= A[i] <= 100
0 <= target <= 300

int threeSumMulti(int* A, int ASize, int target){

}


解题思路：

1、以上四题，类型相似，特别是前三题，只是目标条件不一致（三数和等于0，三数和绝对值与目标值最接近，三数和小于目标值），923较为复杂

2、显而易见的暴力求解方法，使用三重循环，时间复杂度为O(N^3)

3、一般解题思路：先对数组进行排序，然后固定一个值，使用双指针夹逼移动进行求解，注意细节，时间复杂度为O(N^2)

*/



#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//leetcode15
vector<vector<int> > threeSum(vector<int>& nums) {
	//sort the array
	sort(nums.begin(), nums.end());

	//find the answer vector<vector<int>>
	vector<vector<int> > answer;
	int head, tail;
	for(int i = 0; i < (int)nums.size(); i++) {
		
		if(i != 0 && nums[i] == nums[i-1]) continue;//跳过重复的元素
		
		head = i + 1;
		tail = nums.size()-1; 
		
		if(nums[i]>0)//排序后的数组，若基准大于0，则直接退出
			break;
			
		while(head < tail) {
			if(nums[head] + nums[tail] < 0-nums[i]) head++;
			else if(nums[head] + nums[tail] > 0-nums[i]) tail--;
			else
			{
				vector<int> tmp;
				tmp.push_back(nums[i]);
				tmp.push_back(nums[head]);
				tmp.push_back(nums[tail]);

				answer.push_back(tmp);
				
				while(head < tail && nums[head] == nums[head + 1]) head++;
				while(head < tail && nums[tail] == nums[tail + 1]) tail--;
				
				head++;
				tail--;
			}
		}
	}
	return answer;
}

//leetcode259
vector<vector<int> > threeSumSmaller(vector<int>& nums, int target) {
	//sort the array
	sort(nums.begin(), nums.end());

	//find the answer vector<vector<int>>
	vector<vector<int> > answer;
	int head, tail;
	for(int i = 0; i < (int)nums.size(); i++) {
		
		if(i != 0 && nums[i] == nums[i-1]) continue;//跳过重复的元素
		
		head = i + 1;
		tail = nums.size()-1; 
		
		if(nums[i]>target)//排序后的数组，若基准大于0，则直接退出
			break;
			
		while(head < tail) {
			if(nums[head] + nums[tail] >= target-nums[i]) tail--;
			else
			{
				vector<int> tmp;
				tmp.push_back(nums[i]);
				tmp.push_back(nums[head]);
				tmp.push_back(nums[tail]);

				answer.push_back(tmp);
				
				while(head < tail && nums[head] == nums[head + 1]) head++;
				while(head < tail && nums[tail] == nums[tail + 1]) tail--;
				
				//head++;//注意此处代码的不同
				tail--;
			}
		}
	}
	return answer;
}	

//leetcode16
int threeSumClosest(vector<int>& nums, int target) {
	//sort the array
	sort(nums.begin(), nums.end());

	//find the answer closest
	//set a init value 
	int answer = nums[0] + nums[1] + nums[2];
	
	int dis = abs(answer - target);
	
	int head, tail;
	for(int i = 0; i < (int)nums.size(); i++) {
				
		head = i + 1;
		tail = nums.size()-1; 
			
		while(head < tail) {
			int sum3 = nums[i] + nums[head] + nums[tail];
			int temp = abs(sum3 - target);
			
			if(temp < dis)
			{
				dis = temp;
				answer = sum3;
			}
			
			if(sum3 < target)
				head++;
			else if(sum3 > target)
				tail--;
			else
				return sum3;//如果与target相等，则直接返回
		}
	}
	return answer;
}	

//leetcode923
//C语言手撕解法，不使用hashmap、count等数据结构，时间复杂度O(N^2)，空间复杂度O(1)
//其他方法：数学方法，排列组合，求出每个不同元素的出现次数
int threeSumMulti(vector<int>& A, int target) {
        const int MOD=1e9+7;
        int size=A.size();
        sort(A.begin(),A.end());
        long long res=0;
        for(int i=0;i<size;i++){//Ai也可能出现重复
            int twoSum=target-A[i];
            int j=i+1,k=size-1;
            while(j<k){
                if(A[j]+A[k]<twoSum){
                    j++;
                }else if(A[j]+A[k]>twoSum){
                    k--;
                }else if(A[j]!=A[k]){//Aj!=Ak
                    int left=1,right=1;
                    while(j+1<k && A[j]==A[j+1]){//j的重复元素计数
                        left++;
                        j++;
                    }
                    while(k-1>j && A[k]==A[k-1]){//k的重复元素计数
                        right++;
                        k--;
                    }
                    res+=left*right;//计算组合次数
                    j++;
                    k--;
                }else{
                    res+=(k-j+1) * (k-j) / 2;//Aj==Ak，计算组合次数
                    break;
                }
            }
        }
        return res%MOD;
    }



void printvector(vector<vector<int> >& answer)
{
	int i = 0;
	for(i=0; i<answer.size(); i++)
	{
		vector<int> tmpans = answer[i];
		int j = 0;
		for(j=0; j<tmpans.size(); j++)
		{
			cout<<tmpans[j]<<" ";
		}
		cout<<endl;
	}
}



int main() {
	
	vector<int> tmp;
	/*
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(3);
	tmp.push_back(-4);
	tmp.push_back(0);
	tmp.push_back(-1);
	*/
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(2);
	tmp.push_back(2);
	
	vector<vector<int> > answer;
	
	answer = threeSum(tmp);//15
	cout << "answer = " << answer.size() << endl;
	printvector(answer);

	answer = threeSumSmaller(tmp, 0);//259
	cout << "answer = " << answer.size() << endl;
	printvector(answer);
	
	int answer1 = threeSumClosest(tmp, 7);//16
	cout << "answer closest = " << answer1 << endl;
	

	int answer2 = threeSumMulti(tmp, 5);//923
	cout << "answer multi = " << answer2 << endl;
	
	return 0;
}



















