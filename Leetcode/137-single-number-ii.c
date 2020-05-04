/*
137. 只出现一次的数字 II
给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现了三次。找出那个只出现了一次的元素。

说明：

你的算法应该具有线性时间复杂度。 你可以不使用额外空间来实现吗？

示例 1:

输入: [2,2,3,2]
输出: 3
示例 2:

输入: [0,1,0,1,0,1,99]
输出: 99

注解：
利用位运算 或者 构造三进制运算

*/
//解法一——统计bit，普适于N次出现
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int bitNum[32]={0};
        int res=0,len=nums.size();
        for(int i=0;i<32;i++){
            for(int j=0;j<len;j++){
                bitNum[i] +=(nums[j]>>i)&1;
            }
            res |= (bitNum[i]%3)<<i; //通用框架，3可以换成k
        }
        return res;
    }
};

//解法二——三进制，N=3
class Solution {
public:
    int singleNumber(int A[], int n) {
        int ones = 0;
        int twos = 0;
        int threes;
        for (int i=0; i<n;i++){
            int t = A[i];
            twos |= ones&t;
            ones ^= t;
            threes = ones & twos; //默认threes是0;
            ones &= ~threes;
            twos &= ~threes;
        }
        return ones;
    }
};