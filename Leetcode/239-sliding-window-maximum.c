/*
239. �����������ֵ
����һ������ nums����һ����СΪ k �Ļ������ڴ������������ƶ�����������Ҳࡣ��ֻ���Կ����ڻ��������ڵ� k �����֡���������ÿ��ֻ�����ƶ�һλ��

���ػ��������е����ֵ��


���ף�

����������ʱ�临�Ӷ��ڽ��������

ʾ��:

����: nums = [1,3,-1,-3,5,3,6,7], �� k = 3
���: [3,3,5,5,6,7] 
����: 

  �������ڵ�λ��                ���ֵ
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
 

��ʾ��

1 <= nums.length <= 10^5
-10^4 <= nums[i] <= 10^4
1 <= k <= nums.length

˼��

�����Ļ�������˼��

*/

//˫�˶���deque
vector<int> maxSlidingWindow(vector<int>& nums, int k) 
{
	vector<int> res;
	if(k == 0) return res;
	deque<int> window;   //˫�˶��У��Ӷ�ͷ����β ���δ� ���������Ԫ�ص�index ~ ��СԪ�ص�index

	int right = 0;
	while(right < nums.size()){   //����������ÿ����һ�Σ��������һ�����ֵ[����ͷλ�õ�Ԫ��]
		if(!window.empty() && window.front() <= right - k){   //��ͷ���ڴ��ڷ�Χ��
			window.pop_front();
		}
		while(!window.empty() && nums[right] > nums[window.back()]){   //�����Ԫ�رȶ�βԪ�ش�
			window.pop_back();
		}
		window.push_back(right);
		right++;
		if(right >= k) res.push_back(nums[window.front()]);
	}
	return res;
}