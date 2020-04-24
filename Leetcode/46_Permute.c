/*
46、给定一个 没有重复 数字的序列，返回其所有可能的全排列。

示例:

输入: [1,2,3]
输出:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]

假设你总是可以到达数组的最后一个位置。


注解：
1、dfs
深度优先搜索算法（Depth First Search，简称DFS）：一种用于遍历或搜索树或图的算法。 沿着树的深度遍历树的节点，尽可能深的搜索树的分支。当节点v的所在边都己被探寻过或者在搜寻时结点不满足条件，搜索将回溯到发现节点v的那条边的起始节点。整个进程反复进行直到所有节点都被访问为止。属于盲目搜索,最糟糕的情况算法时间复杂度为O(!n)。


//https://blog.csdn.net/weixin_43272781/article/details/82959089

//==============dfs基本模板===============

int check(参数)
{
    if(满足条件)
        return 1;
    return 0;
}
 
void dfs(int step)
{
        判断边界
        {
            相应操作
        }
        尝试每一种可能
        {
               满足check条件
               标记
               继续下一步dfs(step+1)
               恢复初始状态（回溯的时候要用到）
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
	if(n==start)//确定终止条件
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
		swap(A, i, start);//恢复状态
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