/*
829. 连续整数求和
给定一个正整数 N，试求有多少组连续正整数满足所有数字之和为 N?

示例 1:

输入: 5
输出: 2
解释: 5 = 5 = 2 + 3，共有两组连续整数([5],[2,3])求和后为 5。
示例 2:

输入: 9
输出: 3
解释: 9 = 9 = 4 + 5 = 2 + 3 + 4
示例 3:

输入: 15
输出: 4
解释: 15 = 15 = 8 + 7 = 4 + 5 + 6 = 1 + 2 + 3 + 4 + 5
说明: 1 <= N <= 10 ^ 9

*/

#include<stdio.h>
#include<stdlib.h>

int consecutiveNumbersSum(int N){

}

int main()
{
	int left,right;
	int sum;
	int given = 15;
	int count = 0;
	
	for (sum=0, right=1; sum<given; sum +=right, right++)
	;
	printf("\nright =  %d", right);
	
	for(left=1, right--; left<= given/2;)
	if(sum>given)
		sum -=(left++);
	else
	{
		if(sum == given)
		{
		printf("\n%d = sum from %d to %d", given, left, right);
		count++;
		}
		sum +=(++right);
	}
	
	if(count>0)
		printf("\nthere are %d solutions", count);
	else
	    printf("\nno solutions");
}



