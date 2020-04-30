/*
2017年4月21日华为笔试题 德州扑克
https://blog.csdn.net/nameofcsdn/article/details/70339592

一副牌中的五张扑克牌，

牌型1：同花顺：同一花色的顺子

牌型2：四条：四张相同数字+单张

牌型3：葫芦：三张相同的数字+一对

牌型4：同花：同一花色

牌型5：顺子：花色不一样的顺子

牌型6：三条：三张相同+两张单

牌型7：其他

说明：前面的牌型比后面的牌型大

输入由五行组成，如:

3 H
4 H
5 H
6 H
7 H
输出牌型：1

思路：
如果输入10，只读取1，0不管，这样就可以用"234567891JQKA"这13个字母表示13张牌了，写代码方便很多。

然后主要就是要理清楚这7种牌型之间的关系，首先按照最多有多少个数字是一样的，可以分成3大类。

第一类，有4个相同数字，一定是牌型2

第二类，有3个相同数字，是牌型3或6

第三类，没有任何3个数字相同，是牌型1或4或5或7

然后再细分就很简单。

*/


#include <iostream>
using namespace std;
 
char num[5], color[5];
const char an[14] = "234567891JQKA";
 
bool num34(int n)//刚好有3或者4个相同的数字
{
	char c;
	for (int i = 0; i < 5; i++)
	{
		c = num[i];
		int s = 0;
		for (int i = 0; i < 5; i++)if (num[i] == c)s++;
		if (s == n)return true;
	}
	return false;
}
 
bool shun()//是不是顺子
{
	int m = 14;
	for (int i = 0; i < 5; i++)for (int j = 0; j < 13; j++)if (num[i] == an[j] && m>j)m = j;
	for (int j = m; j < m + 5; j++)
	{
		bool flag = true;
		for (int i = 0; i < 5; i++)if (num[i] == an[j])flag = false;
		if (flag)return false;
	}
	return true;
}
 
bool tonghua()//是不是同花
{
	for (int i = 1; i < 5; i++)if (color[i] != color[0])return false;
	return true;
}
 
int f()
{
	if (num34(3))
	{
		char c;
		for (int i = 0; i < 5; i++)
		{
			c = num[i];
			int s = 0;
			for (int i = 0; i < 5; i++)if (num[i] == c)s++;
			if (s == 1)return 6;
		}
		return 3;
	}
	if (num34(4))return 2;
	if (shun())
	{
		if (tonghua())return 1;
		return 5;
	}
	if (tonghua())return 4;
	return 7;
}
 
int main()
{
	char c;
	for (int i = 0; i < 5; i++)
	{
		cin >> num[i];
		if (num[i] == '1')cin >> c;
		cin >> color[i];
	}
	cout << f();
	return 0;
}


测试：

echo "2 H 3 C 6 S 5 S 4 S" | ./test_px
//5

echo "3 H 4 H 5 H 6 H 7 H" | ./test_px
//1
