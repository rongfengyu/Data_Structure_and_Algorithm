/*
57. ��������
����һ�����ص��� ������������ʼ�˵�����������б�

���б��в���һ���µ����䣬����Ҫȷ���б��е�������Ȼ�����Ҳ��ص�������б�Ҫ�Ļ������Ժϲ����䣩��

ʾ�� 1:

����: intervals = [[1,3],[6,9]], newInterval = [2,5]
���: [[1,5],[6,9]]
ʾ�� 2:

����: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
���: [[1,2],[3,10],[12,16]]
����: ������Ϊ�µ����� [4,8] �� [3,5],[6,7],[8,10] �ص���


����˼·

ÿ����������������� [start,end]������������� [a1,b1]��[a2,b2] ���ص���Ҫ���㣺  a1 != a2 ��

�� a1 < a2���� b1 < a2��
�� a1 > a2���� b2 < a1�� 
��Ŀ������˵�������伯���Ѿ���ÿ������Ŀ�ʼ�������У����Ը������伯�� [a1,b1],[a2,b2],[a3,b3],...,[an,bn]��һ���������������� 

a1 < b1 < a2 < b2 < a3 < b3 < ... < an < bn��
Ҫ������µ������� [start,end] �Ὣ���伯�Ϸֳ���������left set��overlapped set��right set�� 

left set��[a1,b1],...,[ai,bi]��0 <= i <= n��
overlapped set��[ai+1,bi+1],...,[aj,bj]��i <= j <= n��
right set��[aj+1,bj+1],...,[an,bn]��i <= j <= n��
ע�⣺�������϶��п���Ϊ�գ�  ���У�

bi < start < end < aj+1;
start <= bi+1 and aj <= end��
overlapped set �еļ���Ҫ���²���������ںϣ��ں�֮��Ľ���� [ min(ai+1, start), max(bj, end) ]��  

�㷨����  

�Ѽ����е����䰴�����ֳ��������������֣�

1.���輯���е�ĳ�������� [am,bm]���²���������� [start,end]�� 

��� bm < start��[am,bm] ���� left set��
��� end < am��[am,bm] ���� right set��
���� [am,bm] ���� overlapped set�� 

2.overlapped set �еļ��Ϻ��²�������� [start,end] �ںϣ��ںϳ�һ������ [min_start, max_end]��

3.������������ϳ��µ����伯�ϣ�left set + [min_start, max_end] + right set�� 

�㷨���Ӷ�

���伯���е�ÿһ������ֻ������һ�Σ�����ʱ�临�Ӷ��� O(n)��  ������Ҫ����Ŀռ�洢�������ֵ����䣬���Կռ临�Ӷ�Ҳ�� O(n)��

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

