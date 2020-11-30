/*
1046. 最后一块石头的重量
有一堆石头，每块石头的重量都是正整数。

每一回合，从中选出两块 最重的 石头，然后将它们一起粉碎。假设石头的重量分别为 x 和 y，且 x <= y。那么粉碎的可能结果如下：

如果 x == y，那么两块石头都会被完全粉碎；
如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x。
最后，最多只会剩下一块石头。返回此石头的重量。如果没有石头剩下，就返回 0。

注解：

堆的应用，大顶堆

拿出最大的两个石头，如果一样重则同时销毁，否则再将差值石头放到堆中，直至最后一块

此题目也可以使用数组和sort函数来模拟堆过程

*/

//Python
class Solution:
    def lastStoneWeight(self, stones: List[int]) -> int:
        heap = [-i for i in stones]   //python默认是小顶堆，因此需要转换下数据
        heapify(heap)
        while len(heap)>1:
            heappush(heap, heappop(heap)-heappop(heap))
        
        return -heap[0]

//C++优先队列 
//优先队列具有队列的所有特性，包括队列的基本操作，只是在这基础上添加了内部的一个排序，它本质是一个堆实现的。
//定义：priority_queue<Type, Container, Functional>，使用基本数据类型时，只需要传入数据类型，默认是大顶堆。
#include<queue>
class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {
        priority_queue<int> pq;
        for(int v : stones) pq.push(v);
        while(pq.size() >= 2){
            int top1 = pq.top(); pq.pop();
            int top2 = pq.top(); pq.pop();
            int v = abs(top1 - top2);
            if(v) pq.push(v);
        }
        return pq.size() == 1 ? pq.top() : 0;
    }
};


/*
1049. 最后一块石头的重量 II
有一堆石头，每块石头的重量都是正整数。

每一回合，从中选出任意两块石头，然后将它们一起粉碎。假设石头的重量分别为 x 和 y，且 x <= y。那么粉碎的可能结果如下：

如果 x == y，那么两块石头都会被完全粉碎；
如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x。
最后，最多只会剩下一块石头。返回此石头最小的可能重量。如果没有石头剩下，就返回 0。

 

示例：

输入：[2,7,4,1,8,1]
输出：1
解释：
组合 2 和 4，得到 2，所以数组转化为 [2,7,1,8,1]，
组合 7 和 8，得到 1，所以数组转化为 [2,1,1,1]，
组合 2 和 1，得到 1，所以数组转化为 [1,1,1]，
组合 1 和 1，得到 0，所以数组转化为 [1]，这就是最优值。
 

提示：

1 <= stones.length <= 30
1 <= stones[i] <= 1000

思路：
	将这些数字分成两拨，使得他们的和的差最小
	
本题最重要的想法：
将数分为两堆，使其中一堆最为接近sum / 2, 从而转化为0-1背包问题
将问题转化为-->石头放或不放进背包 使得背包容量最小（背包容量：sum / 2）

*/

//0-1背包问题
class Solution {
    public int lastStoneWeightII(int[] stones) {
        //在这个集合中，需要保证剩下的一个是最小的，那么是否可以转换成：假设有一个背包，需要把集合中全加进去，并留个最小的，那么对于背包来说，那就是要求最大的重量，那么背包的容量是多少呢？
        // 换一种说法，如何将集合中留下最小呢，假设把里面的石头分为两部分，那么最极端的情况就是两部分相等，那么就为0，也就是说两部分的差值越小越好，那么这就接近于总质量的一半，也就是说背包的容量为和的一半
        //那么01背包的基本操作

        int sum = 0;
        for(int stone : stones){
            sum += stone;
        }
        int target = sum/2;
        int[] dp = new int[target+1];
        for(int i = 0; i < stones.length; i++){
            for(int j = target; j >= stones[i]; j--){
                dp[j] = Math.max(dp[j], dp[j-stones[i]] + stones[i]);//不放，放
            }
        }
        return sum - 2*dp[target]; //放进去最大的，两两相撞，总数减去2倍，那就是剩下最轻的

    }
}