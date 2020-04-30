/*

https://blog.csdn.net/geekmanong/article/details/51524385

题目描述

盒子中放礼物，盒子可以再套盒子，用（）表示一个盒子，A表示礼物，请问最少需要拆多少个盒子才能够拿到礼物

测试案例
输入：
（（（（A）（）））（））
（A）

输出：
4
1


*/

/*
#include <iostream>
#include <string>
using namespace std;
 
int main()
{
     string str;
     while(getline(cin,str))
     {
          int idx;
          for(idx=0;idx!=str.size();idx++)
          {
               if(str[idx]=='A')
                    break;
          }
          int cnt=0;
          for(int i=idx+1;i!=str.size();i++)
          {
               if(str[i]=='(')
                    cnt--;
               if(str[i]==')')
                    cnt++;
          }
          cout<<cnt<<endl;
     }
}
*/

#include <iostream>
#include <string>
using namespace std;
int main()
{
     string str;
     while(getline(cin,str))
     {
          int idx;
		  int cnt=0;
          for(idx=0;idx!=str.size();idx++)
          {
			  if(str[idx]=='(')
                    cnt++;
              if(str[idx]==')')
                    cnt--;
              if(str[idx]=='A')
                    break;
          }
		  
          cout<<cnt<<endl;
     }
}

测试：

echo "(A)\n" | ./test_srlw
//1

echo "(((())((A)))())" | ./test_srlw
//4

echo -e "((((A)()))())\n(A)" | ./test_srlw
//4
//1

【扩展】
虽然可以使用 cin 和 >> 运算符来输入字符串，但它可能会导致一些需要注意的问题。

当 cin 读取数据时，它会传递并忽略任何前导白色空格字符（空格、制表符或换行符）。
一旦它接触到第一个非空格字符即开始阅读，当它读取到下一个空白字符时，它将停止读取。以下面的语句为例：
cin >> namel;

可以输入 "Mark" 或 "Twain"，但不能输入 "Mark Twain"，因为 cin 不能输入包含嵌入空格的字符串。

为了解决这个问题，可以使用一个叫做 getline 的 C++ 函数。此函数可读取整行，包括前导和嵌入的空格，并将其存储在字符串对象中。

getline 函数如下所示：
getline(cin, inputLine);

其中 cin 是正在读取的输入流，而 inputLine 是接收输入字符串的 string 变量的名称。
