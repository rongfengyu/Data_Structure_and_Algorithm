/*
25. K ��һ�鷭ת����
����һ������ÿ k ���ڵ�һ����з�ת�����㷵�ط�ת�������

k ��һ��������������ֵС�ڻ��������ĳ��ȡ�

����ڵ��������� k ������������ô�뽫���ʣ��Ľڵ㱣��ԭ��˳��

ʾ����

�����������1->2->3->4->5

�� k = 2 ʱ��Ӧ������: 2->1->4->3->5

�� k = 3 ʱ��Ӧ������: 3->2->1->4->5

˵����
����㷨ֻ��ʹ�ó����Ķ���ռ䡣
�㲻��ֻ�ǵ����ĸı�ڵ��ڲ���ֵ��������Ҫʵ�ʽ��нڵ㽻����

��Hard��

��ĿҪ����һ���������� k ������ڵ�Ϊ��λ���з�ת��ʲô��˼�أ�

����������һ���ܳ�������ֳɺܶ��С����ÿһ�ݵĳ��ȶ��� k (���һ�ݵĳ������С�� k ����Ҫ��ת)��Ȼ���ÿ��С������з�ת��������з�ת���С����֮ǰ��˳��ƴ����һ��

��ʵ �������Ŀ������Ҫ�ر�ǿ���߼���������Ҫǿ��ϸ��ʵ�֣���Ҳ������ϸ��ʵ������ ����Ȼ���µķ���֪�������Ǻܿ���д��д�žͻ��ҡ�

���������Ŀʵ�ֵ�ʱ��Ҫ����ס����Ҫ�㣺

��һ���ڷ�ת�������ʱ����һ���������β����֪��
�ڶ�����һ���������ͷҲ����֪��
��������ǰ��ת�������ͷβ������֪��

ʱ�临�Ӷȣ�O(n * K) ��
�ռ临�Ӷȣ�O(1)�����˼�������Ľڵ�ָ���⣬��û��ռ�������ռ䡣

*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
public ListNode reverseKGroup(ListNode head, int k) {
    if (head == null || head.next == null || k <= 1) {
        return head;
    }

    ListNode dummy = new ListNode(0);
    dummy.next = head;
    ListNode pointer = dummy;

    while (pointer != null) {
        // ��¼��һ���������β
        ListNode lastGroup = pointer;

        int i = 0;            
        for (; i < k; ++i) {
            pointer = pointer.next;
            if (pointer == null) {
                break;
            }
        }

        // �����ǰ������Ľڵ������� k, �ͽ��з�ת
        // ��֮��˵������β�ˣ��ڵ������������÷�ת
        if (i == k) {
            // ��¼��һ���������ͷ
            ListNode nextGroup = pointer.next;

            // ��ת��ǰ������reverse �������ط�ת���������ͷ
            ListNode reversedList = reverse(lastGroup.next, nextGroup);

            // lastGroup ����һ���������β���� next ָ��ǰ��ת�������ͷ
            // ������Ϊ��ǰ�����Ѿ�����ת��������ָ����Ƿ�ת��������β
            pointer = lastGroup.next;

            // ����һ�������β����ת�������ͷ
            lastGroup.next = reversedList;

            // ��ǰ��ת��������β������һ���������ͷ
            pointer.next = nextGroup;
        }
    }

    return dummy.next;
}

private ListNode reverse(ListNode head, ListNode tail) {
    if (head == null || head.next == null) {
        return head;
    }

    ListNode prev = null, temp = null;
    while ((head != null) && (head != tail)) {
        temp = head.next;
        head.next = prev;
        prev = head;
        head = temp;
    }

    return prev;
}