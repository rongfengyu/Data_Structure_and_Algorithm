/*
46������һ�� û���ظ� ���ֵ����У����������п��ܵ�ȫ���С�

ʾ��:

����: [1,2,3]
���:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]

���������ǿ��Ե�����������һ��λ�á�


ע�⣺
1��dfs
������������㷨��Depth First Search�����DFS����һ�����ڱ�������������ͼ���㷨�� ����������ȱ������Ľڵ㣬����������������ķ�֧�����ڵ�v�����ڱ߶�����̽Ѱ����������Ѱʱ��㲻�������������������ݵ����ֽڵ�v�������ߵ���ʼ�ڵ㡣�������̷�������ֱ�����нڵ㶼������Ϊֹ������äĿ����,����������㷨ʱ�临�Ӷ�ΪO(!n)��


//https://blog.csdn.net/weixin_43272781/article/details/82959089

//==============dfs����ģ��===============

int check(����)
{
    if(��������)
        return 1;
    return 0;
}
 
void dfs(int step)
{
        �жϱ߽�
        {
            ��Ӧ����
        }
        ����ÿһ�ֿ���
        {
               ����check����
               ���
               ������һ��dfs(step+1)
               �ָ���ʼ״̬�����ݵ�ʱ��Ҫ�õ���
        }
}

//=============================



*/


#include<cstdio>
#include<iostream>
#include<vector>
using namespace std;
 
int result=0;
vector<vector<int> > resultNum;
vector<int> resultTemp;


void printfVector(vector<int> vec)
{
	int i = 0;
    for( i = 0; i < vec.size(); i++ )
    {
        cout << vec[i] << " ";
    }
	cout <<endl;
}

void swap(int *nums, int index1, int index2)
{
    int tmp = nums[index1];
    nums[index1] = nums[index2];
    nums[index2] = tmp;
}
    
void permute(int A[],int n, int start)
{
	if(n==start)//ȷ����ֹ����
	{
		resultNum.push_back(resultTemp);
		printfVector(resultTemp);
		result++;
		return;
	}
	
	for(int i=start; i<n; i++)
	{
		swap(A, i, start);
		resultTemp.push_back(A[start]);
		permute(A, n, start+1);
		resultTemp.pop_back();
		swap(A, i, start);//�ָ�״̬
	}
}
 
int main()
{
	int A[]={1,2,3,4};
	int len = sizeof(A)/sizeof(int);
	permute(A, len, 0);
	
	printf("result = %d\n", result);

    return 0;
}