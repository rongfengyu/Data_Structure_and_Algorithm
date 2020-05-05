/*
477. 汉明距离总和
两个整数的 汉明距离 指的是这两个数字的二进制数对应位不同的数量。

计算一个数组中，任意两个数之间汉明距离的总和。

示例:

输入: 4, 14, 2

输出: 6

解释: 在二进制表示中，4表示为0100，14表示为1110，2表示为0010。（这样表示是为了体现后四位之间关系）
所以答案为：
HammingDistance(4, 14) + HammingDistance(4, 2) + HammingDistance(14, 2) = 2 + 2 + 2 = 6.
注意:

数组中元素的范围为从 0到 10^9。
数组的长度不超过 10^4。

注解：

1、两两暴力计算的时间复杂度为o(n^2)，实现上肯定是没有问题，但是当数据量大的时候性能堪忧。

2、公式：每一列求距离产生1的个数 = 本列 1 的个数 * （数字个数 – 本列1的个数）= 本列 1 的个数 * 本列 0 的个数

时间复杂度：时间复杂度：O(N log ⁡C)  其中 C是常数，表示数组中数可能的最大值。

空间复杂度：O(log C)

元素范围最大为10^9，在32bit范围内，2^32 = 4 * 1024 * 1024 * 1024

*/

int totalHammingDistance(int* nums, int numsSize){

}

class Solution {
    public int totalHammingDistance(int[] nums) {
        int len=nums.length;
        int[] bitCount = new int[32];
        if(len <= 1){
            return 0;
        }
        for(int numIndex = 0; numIndex < len; numIndex++){
            for(int bitIndex = 0; bitIndex < 32; bitIndex++){
                bitCount[bitIndex] += nums[numIndex] & 1;
                nums[numIndex] = nums[numIndex] >> 1;
                if(nums[numIndex] == 0){
                    break;
                }
            }
        }
        int oneCount = 0;
        for(int bitIndex = 0; bitIndex < 32; bitIndex++){
            oneCount += bitCount[bitIndex] * (len - bitCount[bitIndex]);
        }
        return oneCount;
    }
}