/*
4. 寻找两个正序数组的中位数
给定两个大小为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。

请你找出这两个正序数组的中位数，并且要求算法的时间复杂度为 O(log(m + n))。

你可以假设 nums1 和 nums2 不会同时为空。

示例 1:

nums1 = [1, 3]
nums2 = [2]

则中位数是 2.0
示例 2:

nums1 = [1, 2]
nums2 = [3, 4]

则中位数是 (2 + 3)/2 = 2.5

注解：

题目实质是查找两个数组合并后的第 (m + n)/2 小元素或者第 (m + n)/2 小元素与第 (m + n)/2 + 1 小元素的平均数。

时间复杂度要求为O(m+n)，这种情况，则是要求二分的思想才能够实现

首先，两个数组都已经是正序排列，每次肯定可以按条件排除剩下所寻找值的一半，

然后配合比较动作进行后面的排除操作，需要对结果进行记录


由于要求的是第 k 小元素，所以可以先求每个数组的 k / 2 小的元素(先假定存在)，分别是 nums1[k / 2 - 1] 和 nums2[k / 2 - 1]。

比较这两个数, 有三种可能性 >, =, <,如果是 = 的情况，那么nums1[k / 2 - 1]就是两个数组合并后的第k小的数

(可以自己简单举个例子试一下，比如[1, 3, 5, 6]和[0, 3, 7]), 如果是 < 的情况，可以得出结论，nums1 中 0 到 k / 2 - 1 的元素均小于两数组的第 k 小元素。

对以上结论简单证明一下,假设 nums1 中存在元素比两数组的第 k 小的元素大，比如nums1[k / 2 - 1]， 
由于 nums1[k / 2 - 1] < nums2[k / 2 - 1] , 那么 nums2[k / 2 - 1] 也比两数组的第 k 小的元素大,这样的话比第 k 小的元素大的元素的个数最少为 m - k / 2 + n - k / 2 + 2 >= m + n - k + 2 。

而实际上比第 k 小的元素大的元素个数只能是 m + n - k . 两者是矛盾的，因此在 nums1[k / 2 - 1] < nums2[k / 2 - 1] 情况下, 
nums1 中 0 到 k / 2 - 1 的元素均小于两数组的第 k 小元素.  取 > 的情况与取 < 的情况类似.根据以上情况，不断进行递归就可以完成求解。

时间复杂度: O(log(m + n))  

*/


class Solution {
public:
    double getK(vector<int> nums1, vector<int> nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        if(m > n) {
            return getK(nums2, nums1, k);
        }
        // boundary
        if(m == 0) {
            return nums2[k - 1];
        } 
        if(k == 1) {
            return min(nums1[0], nums2[0]);
        }
        
        int a = min(k / 2, m), b = k - a;
        if(nums1[a - 1] < nums2[b - 1]) {
            if(nums1.begin() + a != nums1.end()) {
                vector<int>nums(nums1.begin() + a, nums1.end());
                return getK(nums, nums2, k - a);
            } else {
                vector<int>nums;
                return getK(nums, nums2, k - a);
            }
        } else if(nums1[a - 1] == nums2[b - 1]) {
            return nums1[a - 1];
        } else {
            if(nums2.begin() + b != nums2.end()) {
                vector<int>nums(nums2.begin() + b, nums2.end());
                return getK(nums1, nums, k - b);
            } else {
                vector<int>nums;
                return getK(nums1, nums, k - b);
            }
        }
        return 0;
    }
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        if((m + n) % 2 == 0) {
            return (getK(nums1, nums2, (m + n + 1) / 2) + getK(nums1, nums2, (m + n + 2) / 2)) / 2.0;
        } else {
            return getK(nums1, nums2, (m + n + 1) / 2) * 1.0;
        }
    }
};