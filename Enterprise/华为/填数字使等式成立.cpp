/*

将1~9放入9个[]中，使得[][][]+[][][]=[][][]等式成立（dfs）

*/

#include <iostream>
#include <cstdio>
using namespace std;
 
int book[10],total,a[10];
 
void dfs(int temp)
{
    if(temp==10)//确定终止条件
    {
        //等式可以扩展，条件也可修改
        if(a[1]*100+a[2]*10+a[3]+a[4]*100+a[5]*10+a[6]==a[7]*100+a[8]*10+a[9])
        {
            total++;
            printf("%d%d%d+%d%d%d=%d%d%d\n",a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
			return;
        }
    }
    for(int i=1;i<10;i++)
    {
        if(book[i]==0)//数字是否已经用过，填数字无序性
        {
            a[temp]=i;
            book[i]=1;
            dfs(temp+1);
            book[i]=0;
        }
    }
}
 
int main()
{
    dfs(1);
    printf("total=%d\n",total/2);
    return 0;
}
