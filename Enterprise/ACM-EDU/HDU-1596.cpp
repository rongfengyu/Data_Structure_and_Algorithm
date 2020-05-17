/*
find the safest road  http://acm.hdu.edu.cn/showproblem.php?pid=1596
Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)
Total Submission(s): 20324    Accepted Submission(s): 6874


Problem Description
XX星球有很多城市，每个城市之间有一条或多条飞行通道，但是并不是所有的路都是很安全的，
每一条路有一个安全系数s,s是在 0 和 1 间的实数(包括0，1)，一条从u 到 v 的通道P 的安全度为Safe(P) = s(e1)*s(e2)…*s(ek) e1,e2,ek是P 上的边 ，
现在8600 想出去旅游，面对这这么多的路，他想找一条最安全的路。但是8600 的数学不好，想请你帮忙 ^_^
 

Input
输入包括多个测试实例，每个实例包括：
第一行：n。n表示城市的个数n<=1000;
接着是一个n*n的矩阵表示两个城市之间的安全系数，(0可以理解为那两个城市之间没有直接的通道)
接着是Q个8600要旅游的路线,每行有两个数字，表示8600所在的城市和要去的城市
 

Output
如果86无法达到他的目的地，输出"What a pity!",
其他的输出这两个城市之间的最安全道路的安全系数,保留三位小数。
 

Sample Input
3
1 0.5 0.5
0.5 1 0.4
0.5 0.4 1
3
1 2
2 3
1 3
 

Sample Output
0.500
0.400
0.500
*/

/*
最短路径dijkstra变形
*/


#include "cstdio"
#include "cstring"
#include "algorithm"
using namespace std;
#define inf 0x3f3f3f3f
#define MAX 1005
double map1[MAX][MAX],dis[MAX];
int vis[MAX];
///要找安全系数最高的路径
void dijkstra(int s,int n)
{
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++)
        dis[i]=map1[s][i];
    dis[s]=0;
    vis[s]=1;
    double min1;
    int pos;
    for(int i=1;i<n;i++)
    {
        min1=0;
        for(int j=1;j<=n;j++)
        {
            if(!vis[j]&&min1<dis[j])///找最大dis
                min1=dis[pos=j];
        }
        vis[pos]=1;
        for(int j=1;j<=n;j++)
        {
            if(!vis[j]&&dis[j]<dis[pos]*map1[pos][j])///选取更大安全度
                dis[j]=dis[pos]*map1[pos][j];
        }
    }
}
int main()
{
    int n;
    while(~scanf("%d",&n)&&n)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                scanf("%lf",&map1[i][j]);
            }
        }
        int q,s,e;
        scanf("%d",&q);
        for(int i=0;i<q;i++)
        {
            scanf("%d%d",&s,&e);
            dijkstra(s,n);
            if(dis[e]==0)///不能到达，即安全度为0
                printf("What a pity!\n");
            else
                printf("%.3lf\n",dis[e]);
        }
    }
    return 0;
}