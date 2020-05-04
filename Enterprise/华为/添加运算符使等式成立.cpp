/*

在1-9这9个数之间填上+或-，使结果等于目标值(华为机试)

题目来源：华为机试题，难题！

主要思想：深搜dfs，不多把问题简化，知道只剩下一个元素或者不剩下。也算是减而治之。

机试的时候只要求求出有几个即可，我在写的时候把符号都填出来了，存在resultNum中，再次感到C++STL的强大！！！

扩展思考：在1-9这9个数之间填上+或-或*或/，使结果等于目标值（使用下面方法可能计算溢出） 


*/

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

//这个算法是可以在最前面加符号的算法，如：
//-12+3+45-6+78-9=99(没有后面注释掉的if语句)
void sumResult(int A[],int n,int target,int nowValue)
{
	if(n==0 && nowValue==target)//确定终止条件
	{
		resultNum.push_back(resultTemp);
		printfVector(resultTemp);
		result++;
		return;
	}
	
	int addNum=0;
	for(int i=1;i<=n;i++)
	{
		addNum=addNum*10+A[i-1];
		resultTemp.push_back(addNum);
		sumResult(A+i,n-i,target,nowValue+addNum);
		resultTemp.pop_back();
		//if(n!=9)//加上这一句以后，第一个数前面不能加运算符，即始终为+
		{
			resultTemp.push_back(-addNum);
			sumResult(A+i,n-i,target,nowValue-addNum);
			resultTemp.pop_back();
		}
	}
}
 
int main()
{
	int A[9]={1,2,3,4,5,6,7,8,9};
	sumResult(A,9,100,0);
	cout<<result<<endl;
 
    return 0;
}
