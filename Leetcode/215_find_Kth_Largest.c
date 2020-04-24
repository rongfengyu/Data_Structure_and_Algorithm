/*
215、在未排序的数组中找到第 k 个最大的元素。请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。

示例 1:

输入: [3,2,1,5,6,4] 和 k = 2
输出: 5
示例 2:

输入: [3,2,3,1,2,4,5,5,6] 和 k = 4
输出: 4
说明:

你可以假设 k 总是有效的，且 1 ≤ k ≤ 数组的长度。


注解：
1、TopK问题，常见思路是使用堆来解决。

2、求第K大的树，则构建元素个数为K的小顶堆，堆中元素个数小于K时，新元素直接进入堆中；否则，当堆顶小于新元素时，弹出堆顶，将新元素加入堆。

3、由于堆是最小堆，堆顶是堆中的最小元素，新元素都会保证比堆顶小（否则新元素替换堆顶），故堆中K个元素是已扫描元素里最大的K个；堆顶元素即为第K大数。

4、使用有现成优先队列或者堆heap结果的编程语言，很方便解决此类问题，C语言则需要自己造轮子！

*/

class Solution {
    public int findKthLargest(int[] nums, int k) {
        // // 创建一个小顶堆（优先队列模拟）
        PriorityQueue<Integer> heap =
            new PriorityQueue<Integer>();

        // 在堆中维护当前最大k个元素
        for (int i = 0; i < nums.length; i++){
            if(heap.size() < k){
                heap.add(nums[i]);
            }else if (heap.element() < nums[i]){
                heap.poll();
                heap.add(nums[i]);
            }
        }
        return heap.poll();        
  }
}

//时间复杂度O(N*logK)，空间复杂度O(K)