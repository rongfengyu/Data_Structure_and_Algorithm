/*
LeetCode 43. 字符串相乘（Multiply Strings） 大数乘法
 

题目描述
 

给定两个以字符串形式表示的非负整数 num1 和 num2，返回 num1 和 num2 的乘积，它们的乘积也表示为字符串形式。

 

示例 1:

输入: num1 = "2", num2 = "3"
输出: "6"
示例 2:

输入: num1 = "123", num2 = "456"
输出: "56088"
 

说明：

num1 和 num2 的长度小于110。
num1 和 num2 只包含数字 0-9。
num1 和 num2 均不以零开头，除非是数字 0 本身。
不能使用任何标准库的大数类型（比如 BigInteger）或直接将输入转换为整数来处理。
 

解题思路

根据数字乘法的计算规则，从一个数个位开始依次求出与另一个数的乘积并逐位相加。下面以“98”和“99”的乘法计算来说明算法思想。

首先考虑乘积的总位数，两个数相乘的最大位数为两数的位数之和，所以先申请一个结果字符串位数为4，并且每一位都初始化为‘0’
从第一个数的个位数‘8’开始，依次与“99”相乘。在乘法过程中首先初始化每一位置的进位add为0，然后计算出对应单个位的乘积mul，比如第一位8x9=72，然后取其个位与当前位置的数字以及前一位置的进位add相加得到sum，此时sum为2+0+0=2，所以结果字符串的个位数字就为‘2’。当前位置的进位add更新为mul的十位数与sum十位数之和，此时进位add为7+0=7.
计算完一次单个位置的乘法后，最后将当前乘积的前一位更新为add，具体来说8x99=792，但遍历完99后结果只记录了最后两位“92”，此时进位add为7，所以要将前一位更新为7
计算完结果后要判断输出的总位数，因为可能出现结果字符串前几位都是0的情况，找到第一位不是0的数字然后返回之后的字符串。
*/

#include <iostream>

using namespace std;


string multiply(string num1, string num2) 
{
	int l1=num1.size(),l2=num2.size();
	string res(l1+l2,'0');
	
	if(l1==0||l2==0)
		return "";
	
	for(int i=l1-1;i>=0;i--)
	{
		int add=0;
		for(int j=l2-1;j>=0;j--)
		{
			int mul=(num1[i]-'0')*(num2[j]-'0');
			int sum=res[i+j+1]+add+mul%10-'0';
			
			res[i+j+1]='0'+sum%10;//低位
			add=mul/10+sum/10;//高位
		}
		res[i]+=add;
	}
	
	for(int i=0;i<l1+l2;i++)
		if(res[i]!='0')
			return res.substr(i);
	
	return "0";
}


int main() {
	cout << multiply("99", "99") <<endl;
	return 0;
}
