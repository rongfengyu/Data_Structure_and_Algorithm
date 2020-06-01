/*
239. 滑动窗口最大值
给定一个数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。

返回滑动窗口中的最大值。


进阶：

你能在线性时间复杂度内解决此题吗？

示例:

输入: nums = [1,3,-1,-3,5,3,6,7], 和 k = 3
输出: [3,3,5,5,6,7] 
解释: 

  滑动窗口的位置                最大值
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
 

提示：

1 <= nums.length <= 10^5
-10^4 <= nums[i] <= 10^4
1 <= k <= nums.length

思想

基本的滑动窗口思想

*/

//双端队列deque
vector<int> maxSlidingWindow(vector<int>& nums, int k) 
{
	vector<int> res;
	if(k == 0) return res;
	deque<int> window;   //双端队列，从队头到队尾 依次存 窗口内最大元素的index ~ 最小元素的index

	int right = 0;
	while(right < nums.size()){   //后续，窗口每右移一次，都会产生一个最大值[队列头位置的元素]
		if(!window.empty() && window.front() <= right - k){   //队头不在窗口范围内
			window.pop_front();
		}
		while(!window.empty() && nums[right] > nums[window.back()]){   //待入队元素比队尾元素大
			window.pop_back();
		}
		window.push_back(right);
		right++;
		if(right >= k) res.push_back(nums[window.front()]);
	}
	return res;
}