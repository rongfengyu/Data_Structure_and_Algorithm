/*
121. 买卖股票的最佳时机
给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。

如果你最多只允许完成一笔交易（即买入和卖出一支股票一次），设计一个算法来计算你所能获取的最大利润。

注意：你不能在买入股票前卖出股票。

 

示例 1:

输入: [7,1,5,3,6,4]
输出: 5
解释: 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
     注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。
示例 2:

输入: [7,6,4,3,1]
输出: 0
解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。

*/

//121
public class StockProfit {

    public static int maxProfitFor1Time(int prices[]) {
        if(prices==null || prices.length==0) {
            return 0;
        }
        int minPrice = prices[0];
        int maxProfit = 0;
        for (int i = 1; i < prices.length; i++) {
            if (prices[i] < minPrice) {
                minPrice = prices[i];//边走边算，避免最小值出现在右边
            } else if(prices[i] - minPrice > maxProfit){
                maxProfit = prices[i] - minPrice;
            }
        }
        return maxProfit;
    }

    public static void main(String[] args) {
        int[] prices = {9,2,7,4,3,1,8,4};
        System.out.println(maxProfitFor1Time(prices));
    }

}

/*
122. 买卖股票的最佳时机 II
给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你可以尽可能地完成更多的交易（多次买卖一支股票）。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

 

示例 1:

输入: [7,1,5,3,6,4]
输出: 7
解释: 在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。
     随后，在第 4 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出, 这笔交易所能获得利润 = 6-3 = 3 。
示例 2:

输入: [1,2,3,4,5]
输出: 4
解释: 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。
     注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。
     因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。
示例 3:

输入: [7,6,4,3,1]
输出: 0
解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。
 

提示：

1 <= prices.length <= 3 * 10 ^ 4
0 <= prices[i] <= 10 ^ 4

在所有的相对低点买入，在所有的相对高点卖出，累加起来就是最大收益
*/

public int maxProfitForAnyTime(int[] prices) {
        int maxProfit = 0;
        for (int i = 1; i < prices.length; i++) {
            if (prices[i] > prices[i-1])
                maxProfit += prices[i] - prices[i-1];
        }
        return maxProfit;
    }
	
/*
123. 买卖股票的最佳时机 III
给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。

注意: 你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:

输入: [3,3,5,0,0,3,1,4]
输出: 6
解释: 在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3-0 = 3 。
     随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4-1 = 3 。
示例 2:

输入: [1,2,3,4,5]
输出: 4
解释: 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。   
     注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。   
     因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。
示例 3:

输入: [7,6,4,3,1] 
输出: 0 
解释: 在这个情况下, 没有交易完成, 所以最大利润为 0。

思路：
动态规划

这个股票买卖问题，这个问题要求解的是一定天数范围内、一定交易次数限制下的最大收益。

既然限制了股票最多买卖2次，那么股票的交易可以划分为5个阶段：

没有买卖
第1次买入
第1次卖出
第2次买入
第2次卖出

我们把股票的交易阶段设为变量m（用从0到4的数值表示），把天数范围设为变量n。而我们求解的最大收益，受这两个变量影响，用函数表示如下：

最大收益 = F（n，m）（n>=1，0<=m<=4）

既然函数和变量已经确定，接下来我们就要确定动态规划的两大要素：

1.问题的初始状态

假定交易天数的范围只限于第1天，也就是n=1的情况：
1.如果没有买卖，也就是m=0时，最大收益显然是0，也就是 F（1,0）= 0
2.如果有1次买入，也就是m=1时，相当于凭空减去了第1天的股价，最大收益是负的当天股价，也就是 F（1,1）= -price[0]
3.如果有1次买出，也就是m=2时，买卖抵消（当然，这没有实际意义），最大收益是0，也就是 F（1,2）= 0
4.如果有2次买入，也就是m=3时，同m=1的情况，F（1,3）= -price[0]
5.如果有2次卖出，也就是m=4时，同m=2的情况，F（1,4）= 0

2.问题的状态转移方程式
假如天数范围限制从n-1天增加到n天，那么最大收益会有怎样的变化呢？


这取决于现在处于什么阶段（是第几次买入卖出），以及对第n天股价的操作（买入、卖出或观望）。让我们对各个阶段情况进行分析：

1.假如之前没有任何买卖，而第n天仍然观望，那么最大收益仍然是0，即 F（n，0） = 0

2.假如之前没有任何买卖，而第n天进行了买入，那么最大收益是负的当天股价，即 F（n，1）= -price[n-1]
3.假如之前有1次买入，而第n天选择观望，那么最大收益和之前一样，即 F（n，1）= F（n-1，1）
4.假如之前有1次买入，而第n天进行了卖出，那么最大收益是第1次买入的负收益加上当天股价，即那么 F（n，2）= F（n-1，1）+ price[n-1]

5.假如之前有1次卖出，而第n天选择观望，那么最大收益和之前一样，即 F（n，2）= F（n-1，2）

6.假如之前有1次卖出，而第n天进行2次买入，那么最大收益是第1次卖出收益减去当天股价，即F（n，3）= F（n-1，2） - price[n-1]
7.假如之前有2次买入，而第n天选择观望，那么最大收益和之前一样，即 F（n，3）= F（n-1，3）
8.假如之前有2次买入，而第n天进行了卖出，那么最大收益是第2次买入收益减去当天股价，即F（n，4）= F（n-1，3） + price[n-1]

9.假如之前有2次卖出，而第n天选择观望（也只能观望了），那么最大收益和之前一样，即 F（n，4）= F（n-1，4）

F（n，m） = max（F（n-1，m-1）+ price[n-1]，F（n-1，m））

			| 0	（n>0,m=0）
F（n，m）=  | max（F（n-1，m-1）- price[n-1]，F（n-1，m）） （n>1,1<=m<=4,m为奇数）
			| max（F（n-1，m-1）+ price[n-1]，F（n-1，m）） （n>1,1<=m<=4,m为偶数）

*/	
	
//最大买卖次数
private static int MAX_DEAL_TIMES = 2;

public static int maxProfitFor2Time(int[] prices) {
	if(prices==null || prices.length==0) {
		return 0;
	}
	//表格的最大行数
	int n = prices.length;
	//表格的最大列数
	int m = MAX_DEAL_TIMES*2+1;
	//使用二维数组记录数据
	int[][] resultTable = new int[n][m];
	//填充初始状态
	resultTable[0][1] = -prices[0];
	resultTable[0][3] = -prices[0];
	//自底向上，填充数据
	for(int i=1;i<n;++i) {
		for(int j=1;j<m;j++){
			if((j&1) == 1){
				resultTable[i][j] = Math.max(resultTable[i-1][j], resultTable[i-1][j-1]-prices[i]);
			}else {
				resultTable[i][j] = Math.max(resultTable[i-1][j], resultTable[i-1][j-1]+prices[i]);
			}
		}
	}
	//返回最终结果
	return resultTable[n-1][m-1];
}

//【压缩空间】

//最大买卖次数
private static int MAX_DEAL_TIMES = 2;

public static int maxProfitFor2TimeV2(int[] prices) {
	if(prices==null || prices.length==0) {
		return 0;
	}
	//表格的最大行数
	int n = prices.length;
	//表格的最大列数
	int m = MAX_DEAL_TIMES*2+1;
	//使用一维数组记录数据
	int[] resultTable = new int[m];
	//填充初始状态
	resultTable[1] = -prices[0];
	resultTable[3] = -prices[0];
	//自底向上，填充数据
	for(int i=1;i<n;++i) {
		for(int j=1;j<m;j++){
			if((j&1) == 1){
				resultTable[j] = Math.max(resultTable[j], resultTable[j-1]-prices[i]);
			}else {
				resultTable[j] = Math.max(resultTable[j], resultTable[j-1]+prices[i]);
			}
		}
	}
	//返回最终结果
	return resultTable[m-1];
}

在这段代码中，resultTable从二维数组简化成了一维数组。由于最大买卖次数是常量，所以算法的空间复杂度也从O（n）降低到了O（1）。

/*
188. 买卖股票的最佳时机 IV
给定一个整数数组 prices ，它的第 i 个元素 prices[i] 是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

 

示例 1：

输入：k = 2, prices = [2,4,1]
输出：2
解释：在第 1 天 (股票价格 = 2) 的时候买入，在第 2 天 (股票价格 = 4) 的时候卖出，这笔交易所能获得利润 = 4-2 = 2 。
示例 2：

输入：k = 2, prices = [3,2,6,5,0,3]
输出：7
解释：在第 2 天 (股票价格 = 2) 的时候买入，在第 3 天 (股票价格 = 6) 的时候卖出, 这笔交易所能获得利润 = 6-2 = 4 。
     随后，在第 5 天 (股票价格 = 0) 的时候买入，在第 6 天 (股票价格 = 3) 的时候卖出, 这笔交易所能获得利润 = 3-0 = 3 。
 

提示：

0 <= k <= 109
0 <= prices.length <= 1000
0 <= prices[i] <= 1000

思路：题目123的扩展，动态规划
*/

public static int maxProfitForKTime(int[] prices, int k) {
	if(prices==null || prices.length==0 || k<=0) {
		return 0;
	}
	//表格的最大行数
	int n = prices.length;
	//表格的最大列数
	int m = k*2+1;
	//使用一维数组记录数据
	int[] resultTable = new int[m];
	//填充初始状态
	for(int i=1;i<m;i+=2) {
		resultTable[i] = -prices[0];
	}
	//自底向上，填充数据
	for(int i=1;i<n;i++) {
		for(int j=1;j<m;j++){
			if((j&1) == 1){
				resultTable[j] = Math.max(resultTable[j], resultTable[j-1]-prices[i]);
			}else {
				resultTable[j] = Math.max(resultTable[j], resultTable[j-1]+prices[i]);
			}
		}
	}
	//返回最终结果
	return resultTable[m-1];
}