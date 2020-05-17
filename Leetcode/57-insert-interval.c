/*
57. 插入区间
给出一个无重叠的 ，按照区间起始端点排序的区间列表。

在列表中插入一个新的区间，你需要确保列表中的区间仍然有序且不重叠（如果有必要的话，可以合并区间）。

示例 1:

输入: intervals = [[1,3],[6,9]], newInterval = [2,5]
输出: [[1,5],[6,9]]
示例 2:

输入: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
输出: [[1,2],[3,10],[12,16]]
解释: 这是因为新的区间 [4,8] 与 [3,5],[6,7],[8,10] 重叠。


解题思路

每个区间由两部分组成 [start,end]，如果两个区间 [a1,b1]和[a2,b2] 不重叠需要满足：  a1 != a2 且

若 a1 < a2，则 b1 < a2；
若 a1 > a2，则 b2 < a1； 
题目描述中说明，区间集合已经按每个区间的开始升序排列，所以给定区间集合 [a1,b1],[a2,b2],[a3,b3],...,[an,bn]，一定满足以下条件： 

a1 < b1 < a2 < b2 < a3 < b3 < ... < an < bn。
要插入的新的区间是 [start,end] 会将区间集合分成三个部分left set，overlapped set和right set： 

left set：[a1,b1],...,[ai,bi]，0 <= i <= n；
overlapped set：[ai+1,bi+1],...,[aj,bj]，i <= j <= n；
right set：[aj+1,bj+1],...,[an,bn]，i <= j <= n；
注意：三个集合都有可能为空！  并有：

bi < start < end < aj+1;
start <= bi+1 and aj <= end。
overlapped set 中的集合要和新插入的区间融合，融合之后的结果是 [ min(ai+1, start), max(bj, end) ]。  

算法流程  

把集合中的区间按条件分成上述的三个部分：

1.假设集合中的某个区间是 [am,bm]，新插入的区间是 [start,end]。 

如果 bm < start，[am,bm] 属于 left set；
如果 end < am，[am,bm] 属于 right set；
否则 [am,bm] 属于 overlapped set。 

2.overlapped set 中的集合和新插入的区间 [start,end] 融合，融合成一个区间 [min_start, max_end]。

3.将三个部分组合成新的区间集合：left set + [min_start, max_end] + right set。 

算法复杂度

区间集合中的每一个区间只被处理一次，所以时间复杂度是 O(n)；  由于需要另外的空间存储三个部分的区间，所以空间复杂度也是 O(n)。

*/

/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> left, right;
        int start=newInterval.start;
        int end=newInterval.end;
        for(const auto& interval: intervals){
            if(start>interval.end) left.emplace_back(interval);
            else if(end<interval.start) right.emplace_back(interval);
            else{
                //merge intervals
                start=min(start, interval.start);
                end=max(end, interval.end);
            }
        }
        //result: left + newInterval + right
        left.emplace_back(Interval(start, end));
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }
};

