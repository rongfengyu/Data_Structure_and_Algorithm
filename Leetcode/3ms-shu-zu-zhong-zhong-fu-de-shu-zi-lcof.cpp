/*
面试题03. 数组中重复的数字
找出数组中重复的数字。


在一个长度为 n 的数组 nums 里的所有数字都在 0～n-1 的范围内。数组中某些数字是重复的，但不知道有几个数字重复了，也不知道每个数字重复了几次。请找出数组中任意一个重复的数字。

示例 1：

输入：
[2, 3, 1, 0, 2, 5, 3]
输出：2 或 3 
 

限制：

2 <= n <= 100000


注意题目描述：一个长度为 n 的数组 nums 里的所有数字都在 0～n-1 的 范围内，这个 范围 恰好与数组的下标可以一一对应。

所以我们可以执行某种操作，使索引与值一一对应，即索引 0 的值为 0，索引 1 的值为 1。而一旦某个索引的值不只一个，则找到了重复的数字，也即发生了 哈希冲突。

时间复杂度 O(N)

注意参考代码里面的关键字 continue，这表示在 while 的一次循环里面，只有这次循环将 索引(i) 与 索引值(num[i]) 匹配到了，才会执行下一次循环。

在每一次的循环过程中，索引(i) 与 索引值(num[i]) 匹配到后，在后续的循环过程中不会操作它们，所以虽然一开始的循环过程中，执行的交换操作较多，但在后续的循环过程中根本不需要再执行操作了。

根据均摊复杂度分析 ，总的时间复杂度为  O(N) ，N 为数组的长度。

空间复杂度

使用常数复杂度的额外空间，为  O(1)。
*/

class Solution {
    public int findRepeatNumber(int[] nums) {
        //设索引初始值为 i = 0
        int i = 0;
        //遍历整个数组 nums 
        while(i < nums.length) {
            //索引 i 的值为 i,无需执行交换操作，查看下一位
            if(nums[i] == i) {
                i++;
                continue;
            }
            //索引 nums[i] 处的值也为 nums[i]，即找到一组相同值，返回 nums[i] 即可
            if(nums[nums[i]] == nums[i]) return nums[i];
            //执行交换操作，目的是为了使索引与值一一对应，即索引 0 的值为 0，索引 1 的值为 1
            int tmp = nums[i];
            nums[i] = nums[tmp];
            nums[tmp] = tmp;
        }
        //如果遍历整个数组都没有找到相同的值，返回 -1
        return -1;
    }
}