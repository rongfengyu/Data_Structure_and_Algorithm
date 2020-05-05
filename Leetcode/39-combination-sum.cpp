/*
39. ����ܺ�
����һ�����ظ�Ԫ�ص����� candidates ��һ��Ŀ���� target ���ҳ� candidates �����п���ʹ���ֺ�Ϊ target ����ϡ�

candidates �е����ֿ����������ظ���ѡȡ��

˵����

�������֣����� target��������������
�⼯���ܰ����ظ�����ϡ� 
ʾ�� 1:

����: candidates = [2,3,6,7], target = 7,
����⼯Ϊ:
[
  [7],
  [2,2,3]
]
ʾ�� 2:

����: candidates = [2,3,5], target = 8,
����⼯Ϊ:
[
  [2,2,2,2],
  [2,3,3],
  [3,5]
]


ע�⣺

�����㷨����֦


*/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
 
class Solution {
public:
    vector<vector<int> > combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        result.clear();
        helper(candidates, vector<int>(), 0, target);
        return result;
    }
private:
    void helper(vector<int>& candidates, vector<int> cur, int start, int target){
        if(target==0) {//����ж�
            result.push_back(cur);
            return;
        }
        for(int i=start; i<candidates.size(); i++){
            if(target<candidates[i]) break;//��֦
			
            cur.push_back(candidates[i]);
            helper(candidates, cur, i, target-candidates[i]);//�޸�i�����Կ����Ƿ����ʹ���ظ�ֵ
            cur.pop_back();
        }
    }
    vector<vector<int> > result;
};

void printfVector(vector<int> vec)
{
	int i = 0;
	for( i = 0; i < vec.size(); i++ )
   {
    cout << vec[i] << " ";
   }
	cout <<endl;
}

int main()
{
	Solution solution;
	vector<int> A;
	A.push_back(2);
	A.push_back(3);
	A.push_back(6);
	A.push_back(7);
	
	vector<vector<int> > result;
	result = solution.combinationSum(A,6);
	
	int i = 0;
	for( i = 0; i < result.size(); i++ )
   {
		printfVector(result[i]);
   }
 
    return 0;
}
