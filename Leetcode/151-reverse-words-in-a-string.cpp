/*
151. 翻转字符串里的单词
给定一个字符串，逐个翻转字符串中的每个单词。

示例 1：

输入: "the sky is blue"
输出: "blue is sky the"
示例 2：

输入: "  hello world!  "
输出: "world! hello"
解释: 输入字符串可以在前面或者后面包含多余的空格，但是反转后的字符不能包括。
示例 3：

输入: "a good   example"
输出: "example good a"
解释: 如果两个单词间有多余的空格，将反转后单词间的空格减少到只含一个。
 

说明：

无空格字符构成一个单词。
输入字符串可以在前面或者后面包含多余的空格，但是反转后的字符不能包括。
如果两个单词间有多余的空格，将反转后单词间的空格减少到只含一个。
 

进阶：

请选用 C 语言的用户尝试使用 O(1) 额外空间复杂度的原地解法。

编程珠玑中的三步翻转法

三步反转法：先将每个单词分成独立的几部分，然后分别对它们进行翻转，返回将整个字符串进行翻转。（最后一步的翻转其实是可以省略的，直接将字符串从后往前输出即可）

    输入：“I am a student.”

中间结果：“I ma a .tneduts”

    输出：“student. a am I”


*/

//三步反转法
#include<iostream>
#include<string.h>
 
using namespace std;
 
void print(char *s, int m)
{	
	for (int i = 0; i < m; i++)
	{		
		cout << s[i];
	}	
	cout << endl;	
}
 
//将每个单词进行翻转
void reverse(char *s,int low,int high)
{
	while (low < high)
	{
		int tmp = s[high];
		s[high] = s[low];
		s[low] = tmp;
		low++;
		high--;
	}
}
 
int main()
{
	int num = 0;
	int low, high;
	//cout << "请输入一个字符串：";
	char a[] = "I am a student.";
	//单词的长度
	int n = strlen(a);
	cout << "n=" << n << endl;
	//显示未翻转前的字符串
	print(a, n);
	//将字符串分为独立的几个单词,并分别进行翻转
	for (int i = 0; i <= n; i++)
	{
		if (a[i] == ' '||a[i]=='\0')
		{
			//单词翻转
			reverse(a,i-num,i-1);
			num = 0;
		}
		else
		{
			num++;
		}
	}
	//中间结果
	print(a, n);
	//显示翻转之后的字符串
	for (int i = n-1; i >=0; i--)
	{
		cout << a[i];
	}
	cout << endl;
	
	return 0;
}
