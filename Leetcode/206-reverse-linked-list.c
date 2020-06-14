/*
206. ��ת����
��תһ��������

ʾ��:

����: 1->2->3->4->5->NULL
���: 5->4->3->2->1->NULL
����:
����Ե�����ݹ�ط�ת�������ܷ������ַ����������⣿
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
    ListNode* reverseList(ListNode* head) {

    }
};


����һ������
����������� 1 �� 2 �� 3 �� ?��������Ҫ�����ĳ� ? �� 1 �� 2 �� 3��

�ڱ����б�ʱ������ǰ�ڵ�� next ָ���Ϊָ��ǰһ��Ԫ�ء����ڽڵ�û����������һ���ڵ㣬��˱������ȴ洢��ǰһ��Ԫ�ء��ڸ�������֮ǰ������Ҫ��һ��ָ�����洢��һ���ڵ㡣��Ҫ��������󷵻��µ�ͷ���ã�

public ListNode reverseList(ListNode head) {
    ListNode prev = null;
    ListNode curr = head;
    while (curr != null) {
        ListNode nextTemp = curr.next;
        curr.next = prev;
        prev = curr;
        curr = nextTemp;
    }
    return prev;
}

���Ӷȷ���
ʱ�临�Ӷȣ�O(n)��
�ռ临�Ӷȣ�O(1)��

���������ݹ�
�ݹ�汾��΢����һЩ����ؼ����ڷ������������б�����ಿ���Ѿ�����ת�������Ҹ���η�ת��ǰ��Ĳ��֣�

public ListNode reverseList(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode p = reverseList(head.next);
    head.next.next = head;
    head.next = null;
    return p;
}
���Ӷȷ���
ʱ�临�Ӷȣ����� n ���б�ĳ��ȣ���ôʱ�临�Ӷ�Ϊ O(n)��
�ռ临�Ӷȣ�O(n)������ʹ�õݹ飬����ʹ����ʽջ�ռ䡣�ݹ���ȿ��ܻ�ﵽ n �㡣

