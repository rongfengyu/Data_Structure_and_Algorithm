/*
55. ��Ծ��Ϸ
����һ���Ǹ��������飬�����λ������ĵ�һ��λ�á�

�����е�ÿ��Ԫ�ش������ڸ�λ�ÿ�����Ծ����󳤶ȡ�

�ж����Ƿ��ܹ��������һ��λ�á�

ʾ�� 1:

����: [2,3,1,1,4]
���: true
����: ���ǿ������� 1 ������λ�� 0 ���� λ�� 1, Ȼ���ٴ�λ�� 1 �� 3 ���������һ��λ�á�
ʾ�� 2:

����: [3,2,1,0,4]
���: false
����: �������������ܻᵽ������Ϊ 3 ��λ�á�����λ�õ������Ծ������ 0 �� ��������Զ�����ܵ������һ��λ�á�


ע�⣺
�Ա�leetcode45�����Ƶ��ǲ�ͬ�������˵�����������ж��Ƿ���Ե����Ȼʹ��̰���㷨���������飬�жϸ��Ƿ�Χ�Ƿ���Ե����β����

1��ÿ��Ԫ���ǿ�����Ծ����󳤶ȣ������Ǳ�����Ծ�ĳ��ȣ�

2����Ҫ֪���ϴ��ܵ����λ�ã���Ϊ��һ�ε���㡣Ȼ����Ҫ֪���˴��ܵ������Զ��λ�ã���Ϊ��һ�ε���㡣

3��̰���㷨���������飬�жϸ��Ƿ�Χ�Ƿ���Ե����β���ɣ����Ƹ�Ⱦ������һ��˼�룩

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
	steps.push_back(0);  //ע����һ�п��������û����Ч
	steps.push_back(4);
	Solution solve;
	bool result = solve.canJump(steps);
	if(result)
        	printf("true\n");
    	else
        	printf("false\n");
}
