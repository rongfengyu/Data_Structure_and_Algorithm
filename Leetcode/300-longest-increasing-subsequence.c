/*
300. �����������
����һ��������������飬�ҵ���������������еĳ��ȡ�

ʾ��:

����: [10,9,2,5,3,7,101,18]
���: 4 
����: ��������������� [2,3,7,101]�����ĳ����� 4��
˵��:

���ܻ��ж�������������е���ϣ���ֻ��Ҫ�����Ӧ�ĳ��ȼ��ɡ�
���㷨��ʱ�临�Ӷ�Ӧ��Ϊ O(n2) ��
����: ���ܽ��㷨��ʱ�临�ӶȽ��͵� O(n log n) ��?

����: 

����������У�Longest Increasing Subsequence����д LIS���Ƿǳ������һ���㷨���⣬
�Ƚ������뵽���Ƕ�̬�滮�ⷨ��ʱ�临�Ӷ� O(N^2)�����ǽ������������ǳ����������״̬ת�Ʒ��̣����д����̬�滮�ⷨ��

�Ƚ����뵽�������ö��ֲ��ң�ʱ�临�Ӷ��� O(NlogN)������ͨ��һ�ּ򵥵�ֽ����Ϸ�����������������Ľⷨ��

�ⷨһ ���� DP
����dp[i] ��ʾ�� nums[i] �������β������������еĳ��ȡ�

 base case��dp[i] ��ʼֵΪ 1����Ϊ�� nums[i] ��β�����������������Ҫ�������Լ���
 
���ս���������е���󳤶ȣ�Ӧ���� dp �����е����ֵ��
int res = 0;
for (int i = 0; i < dp.size(); i++) {
    res = Math.max(res, dp[i]);
}
return res;

�ܽ�һ������ҵ���̬�滮��״̬ת�ƹ�ϵ��

1����ȷ dp �����������ݵĺ��塣��һ�������κζ�̬�滮���ⶼ����Ҫ��������õ����߲������������谭֮��Ĳ��衣

2������ dp ����Ķ��壬������ѧ���ɷ���˼�룬���� dp[0...i-1] ����֪����취��� dp[i]��һ����һ����ɣ�������Ŀ�����ͽ���ˡ�

������޷������һ�����ܿ��ܾ��� dp ����Ķ��岻��ǡ������Ҫ���¶��� dp ����ĺ��壻���߿����� dp ����洢����Ϣ���������������Ƴ���һ���Ĵ𰸣���Ҫ�� dp ��������ɶ�ά����������ά���顣

�ⷨ�� ���� ���ֲ���
ʱ�临�Ӷ�Ϊ O(NlogN)������˵ʵ���������˻����벻�����ֽⷨ��Ҳ�����ĳЩֽ����Ϸ���˿������������

����������к�һ�ֽ��� patience game ��ֽ����Ϸ�йأ�������һ�����򷽷��ͽ��� patience sorting���������򣩡�

*/

//DP
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        if(n == 0)
            return 0;
        vector<int> dp(n, 1);
        int max_len = 1;
        
        for(int i = 1; i < n; i++)
        {
            for(int j = 0; j < i; j++)
            {
                if(nums[j] < nums[i])
                {
                    dp[i] = max(dp[i], dp[j]+1);
                    //dp[i] = dp[j]+1;
                }
            }
            max_len = max(max_len, dp[i]);
        }
		
		for(int i = 0; i < n; i++)
			cout<<dp[i]<<endl;
        return max_len;
    }
};

int main() {
	
	vector<int> steps;
	steps.push_back(10);
	steps.push_back(9);
	steps.push_back(2);
	steps.push_back(5);
	steps.push_back(2);
	steps.push_back(7);
	steps.push_back(101);
	steps.push_back(18);

	Solution solve;
	int result = solve.lengthOfLIS(steps);
		
	cout << "result= " << result << endl;
	return 0;
}

//Binary Search
public int lengthOfLIS(int[] nums) {
    int[] top = new int[nums.length];
    // �ƶ�����ʼ��Ϊ 0
    int piles = 0;
    for (int i = 0; i < nums.length; i++) {
        // Ҫ������˿���
        int poker = nums[i];

        /***** �������߽�Ķ��ֲ��� *****/
        int left = 0, right = piles;
        while (left < right) {
            int mid = (left + right) / 2;
            if (top[mid] > poker) {
                right = mid;
            } else if (top[mid] < poker) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        /*********************************/

        // û�ҵ����ʵ��ƶѣ��½�һ��
        if (left == piles) piles++;
        // �������Ʒŵ��ƶѶ�
        top[left] = poker;
    }
    // �ƶ������� LIS ����
    return piles;
}
