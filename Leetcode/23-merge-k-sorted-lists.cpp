/*
23. �ϲ�K����������
�ϲ� k �������������غϲ������������������������㷨�ĸ��Ӷȡ�

ʾ��:

����:
[
  1->4->5,
  1->3->4,
  2->6
]
���: 1->1->2->3->4->4->5->6

��Hard��

���˼·��1��

�ȼ������е�Ԫ�ظ�����n���������ǿ���ȷ��һ�����Ӷ��Ͻ磬���ǰ���k�������е����нڵ㱣�����������������������Ӷ���Ҫ��������һ��O(nlgn)��������

���˼·��2��

������k���������Ѿ���������������Ż����Ͻ���㷨����������������ͬһ�������Ԫ��a,b��a<b����Ȼ������Ҫ�鲢��һ��Ŀ��������ô��aû�б��Ž�Ŀ������֮ǰb�϶����ᱻ�Ž�Ŀ����������Ŀ������ÿ�ηŽ�ȥ��Ԫ�ر�Ȼ��k������ǰ��ͷԪ���е���Сֵ������������Ҫά��һ�����ݽṹ��������ݽṹ��Ҫ֧�֣�

�ܿ��ٲ���k��Ԫ�ص���Сֵ��
֧�ֲ��룬ɾ����СԪ�ء�
��Щ�����Ĳ����������е�ƽ����������ɡ�������ƽ��������е���С���ˣ���ЩҪ�����ǿ�����һ�����ֿ죨����С���ֺã���̸��Ӷȵͣ���ʵ�֣�������C++�жѾͷ�װ����priority_queue,����ֱ�������á�����ÿ�ξ�ֻ����k��Ԫ����ѡ��һ����Сֵ��������O(lgk)���������Ǿ�ʵ����һ��O(nlgk)���㷨�����ڼ����Ե��Ͻ��㷨��


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