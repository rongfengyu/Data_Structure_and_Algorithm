/*
4. Ѱ�����������������λ��
����������СΪ m �� n �����򣨴�С�������� nums1 �� nums2��

�����ҳ������������������λ��������Ҫ���㷨��ʱ�临�Ӷ�Ϊ O(log(m + n))��

����Լ��� nums1 �� nums2 ����ͬʱΪ�ա�

ʾ�� 1:

nums1 = [1, 3]
nums2 = [2]

����λ���� 2.0
ʾ�� 2:

nums1 = [1, 2]
nums2 = [3, 4]

����λ���� (2 + 3)/2 = 2.5

ע�⣺

��Ŀʵ���ǲ�����������ϲ���ĵ� (m + n)/2 СԪ�ػ��ߵ� (m + n)/2 СԪ����� (m + n)/2 + 1 СԪ�ص�ƽ������

ʱ�临�Ӷ�Ҫ��ΪO(m+n)���������������Ҫ����ֵ�˼����ܹ�ʵ��

���ȣ��������鶼�Ѿ����������У�ÿ�ο϶����԰������ų�ʣ����Ѱ��ֵ��һ�룬

Ȼ����ϱȽ϶������к�����ų���������Ҫ�Խ�����м�¼


����Ҫ����ǵ� k СԪ�أ����Կ�������ÿ������� k / 2 С��Ԫ��(�ȼٶ�����)���ֱ��� nums1[k / 2 - 1] �� nums2[k / 2 - 1]��

�Ƚ���������, �����ֿ����� >, =, <,����� = ���������ônums1[k / 2 - 1]������������ϲ���ĵ�kС����

(�����Լ��򵥾ٸ�������һ�£�����[1, 3, 5, 6]��[0, 3, 7]), ����� < ����������Եó����ۣ�nums1 �� 0 �� k / 2 - 1 ��Ԫ�ؾ�С��������ĵ� k СԪ�ء�

�����Ͻ��ۼ�֤��һ��,���� nums1 �д���Ԫ�ر�������ĵ� k С��Ԫ�ش󣬱���nums1[k / 2 - 1]�� 
���� nums1[k / 2 - 1] < nums2[k / 2 - 1] , ��ô nums2[k / 2 - 1] Ҳ��������ĵ� k С��Ԫ�ش�,�����Ļ��ȵ� k С��Ԫ�ش��Ԫ�صĸ�������Ϊ m - k / 2 + n - k / 2 + 2 >= m + n - k + 2 ��

��ʵ���ϱȵ� k С��Ԫ�ش��Ԫ�ظ���ֻ���� m + n - k . ������ì�ܵģ������ nums1[k / 2 - 1] < nums2[k / 2 - 1] �����, 
nums1 �� 0 �� k / 2 - 1 ��Ԫ�ؾ�С��������ĵ� k СԪ��.  ȡ > �������ȡ < ���������.����������������Ͻ��еݹ�Ϳ��������⡣

ʱ�临�Ӷ�: O(log(m + n))  

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