/*
23. 合并K个排序链表
合并 k 个排序链表，返回合并后的排序链表。请分析和描述算法的复杂度。

示例:

输入:
[
  1->4->5,
  1->3->4,
  2->6
]
输出: 1->1->2->3->4->4->5->6

【Hard】

题解思路（1）

先假设所有的元素个数是n。首先我们可以确定一个复杂度上界，就是把这k个链表中的所有节点保存下来排序，这样的做法复杂度主要在排序，是一个O(nlgn)的做法。

题解思路（2）

接下用k个链表本身已经有序这个限制来优化下上界的算法。假设有两个来自同一个链表的元素a,b（a<b），然后我们要归并到一个目标链表，那么在a没有被放进目标链表之前b肯定不会被放进目标链表。所以目标链表每次放进去的元素必然是k个链表当前表头元素中的最小值。所以我们需要维护一个数据结构，这个数据结构需要支持：

能快速查找k个元素的最小值；
支持插入，删除最小元素。
这些基本的操作几乎所有的平衡树都能完成。但是用平衡树解决有点大材小用了，这些要求我们可以用一个堆又快（常数小）又好（编程复杂度低）的实现，比如在C++中堆就封装成了priority_queue,可以直接拿来用。这样每次就只需在k个元素中选择一个最小值，单次是O(lgk)，于是我们就实现了一个O(nlgk)的算法，优于简单无脑的上界算法。


*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    struct node {
        int val, id;
        bool operator < (const node & _) const {
            return val > _.val;
        }
    };
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue <node>que;
        while (!que.empty ()) que.pop ();
        int n = lists.size ();
        ListNode *head = new ListNode (0), *p; p = head;
        
        for (int i = 0; i < n; i++) {
            if (lists[i] != NULL) {
                que.push ((node) {lists[i]->val, i});
                lists[i] = lists[i]->next;
            }
        }
        
        while (!que.empty ()) {
            node tmp = que.top (); que.pop ();
            if (lists[tmp.id] != NULL) {
                que.push ((node) {lists[tmp.id]->val, tmp.id});
                lists[tmp.id] = lists[tmp.id]->next;
            }
            p->next = new ListNode (tmp.val);
            p = p->next;
        }
        return head->next;
    }
};