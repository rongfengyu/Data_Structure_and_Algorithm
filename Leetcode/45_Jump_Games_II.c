/*
45. ��Ծ��Ϸ II
����һ���Ǹ��������飬�����λ������ĵ�һ��λ�á�

�����е�ÿ��Ԫ�ش������ڸ�λ�ÿ�����Ծ����󳤶ȡ�

���Ŀ����ʹ�����ٵ���Ծ����������������һ��λ�á�

ʾ��:

����: [2,3,1,1,4]
���: 2
����: �������һ��λ�õ���С��Ծ���� 2��
     ���±�Ϊ 0 �����±�Ϊ 1 ��λ�ã��� 1 ����Ȼ���� 3 ��������������һ��λ�á�
˵��:

���������ǿ��Ե�����������һ��λ�á�


ע�⣺

������Ҫ��˵���������ǿ��Ե�����������һ��λ�ã�����  �����ų�leetcode55�пӵ��������
��{2,1,1,3,2,1,0,1}����ʱ�յ㲻�ɵ���

1��ÿ��Ԫ���ǿ�����Ծ����󳤶ȣ������Ǳ�����Ծ�ĳ��ȣ�

2����Ҫ֪���ϴ��ܵ����λ�ã���Ϊ��һ�ε���㡣Ȼ����Ҫ֪���˴��ܵ������Զ��λ�ã���Ϊ��һ�ε���㡣

3��̰���㷨������������ʱ��ῼ�ǵ���һ����������һ����������Զ��̰�Ĳ��ԣ�������ÿһ����ѡ��
ɨ�����飬��ȷ����ǰ��Զ�ܸ��ǵĽڵ㡣Ȼ�����ɨ�裬ֱ����ǰ��·�̳�������һ������ĸ��Ƿ�Χ����ô���¸��Ƿ�Χ��ͬʱ������������Ϊ�����Ǿ����˶�һ�����ܼ���ǰ���ġ�
�����˵�����������ȡÿ����Զ��greedy��

*/

#include <stdio.h>
	
int jump(int* nums, int numsSize)
{
    int cnt = 0;
    if (nums == NULL || numsSize == 0) {
        return cnt;
    }
    
    int maxPos = 0; //�´���Զ�ܵ����λ��
    int right = 0;   //���δ���Զ�ܵ����λ��
    
    for (int i = 0; i < numsSize - 1; i++) {
        maxPos = (nums[i] + i) > maxPos ? (nums[i] + i) : maxPos;//ʹ����Ԫ�����������������ı仯����һ���ܸ߶˵��÷�
        printf("i=%d, maxPos = %d\n", i, maxPos);

        if (i == right) {
            cnt++;//���ڼ����ͺ�

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


����չ��
��������������ܲ���0����ӣ�Ӧ�����޽�ģ�
{2,1,1,3,2,1,0,1};

#include <stdio.h>
#include <vector>
using namespace std;
class Solution{
public:
	int jumpGame2(vector<int> steps){
		//�������С�ڵ���1�����ش���1��һ��������
		if(steps.size() <= 1)
			return 0;
		//current_index = steps[a]+a;
		int current_index = steps[0];
		//pre_index = (a,b] ֮�����ȥ������Զ����
		int pre_index = steps[0];
		//��Ծ����
		int times = 0;
		//��¼ÿ������һ��
		int theBestChoise = 0;
		for(int i = 1;i < steps.size();i++){
			//ָ������ b+1��λ�ã����ò�����һ��
			if(i > current_index){
				current_index = pre_index;
				//���ò���һ����ֻ���ߵ�һ��������ʱ��������
				if(steps[current_index] == 0)
                    return 600;
				printf("��Ծ����%d������\n",theBestChoise);
				times++;
			}
			//��pre_index �ܷ񱻳���
			if(pre_index < i+steps[i]){
				theBestChoise = i;
				pre_index = i+steps[i];
                if(pre_index >= steps.size()-1){ //�ܹ����յ��ˣ�ֱ���˳�
                    times = times + 2; //����������������
                    printf("��Ծ����%d������,Ȼ��ֱ�������յ�\n",theBestChoise);
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
	//steps.push_back(0);  //ע����һ�п��������û����Ч
	steps.push_back(2);
	steps.push_back(3);
	steps.push_back(5);
	steps.push_back(2);
	Solution solve;
	int result = solve.jumpGame2(steps);
	if(result == 0)
        printf("��ֻ��һ�����ӻ���һ�����Ӷ�û�У���0����");
    else if(result == 600)
        printf("�㲻�ò�����һ�����У�����һ����");
    else
        printf("�ܹ���Ҫ��Ծ%d����",result);
}