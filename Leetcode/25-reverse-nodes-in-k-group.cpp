/*
25. K 个一组翻转链表
给你一个链表，每 k 个节点一组进行翻转，请你返回翻转后的链表。

k 是一个正整数，它的值小于或等于链表的长度。

如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。

示例：

给你这个链表：1->2->3->4->5

当 k = 2 时，应当返回: 2->1->4->3->5

当 k = 3 时，应当返回: 3->2->1->4->5

说明：
你的算法只能使用常数的额外空间。
你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。

【Hard】

题目要求在一个链表中以 k 个链表节点为单位进行反转，什么意思呢？

你可以想象把一个很长的链表分成很多个小链表，每一份的长度都是 k (最后一份的长度如果小于 k 则不需要反转)，然后对每个小链表进行反转，最后将所有反转后的小链表按之前的顺序拼接在一起。

其实 链表的题目并不需要特别强的逻辑推理，它主要强调细节实现，难也是难在细节实现上面 ，虽然大致的方向知道，但是很可能写着写着就会乱。

所以这个题目实现的时候要把握住几个要点：

第一，在反转子链表的时候，上一个子链表的尾必须知道
第二，下一个子链表的头也必须知道
第三，当前反转的链表的头尾都必须知道

时间复杂度：O(n * K) 。
空间复杂度：O(1)。除了几个必须的节点指针外，并没有占用其它空间。

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
        // 记录上一个子链表的尾
        ListNode lastGroup = pointer;

        int i = 0;            
        for (; i < k; ++i) {
            pointer = pointer.next;
            if (pointer == null) {
                break;
            }
        }

        // 如果当前子链表的节点数满足 k, 就进行反转
        // 反之，说明程序到尾了，节点数不够，不用反转
        if (i == k) {
            // 记录下一个子链表的头
            ListNode nextGroup = pointer.next;

            // 反转当前子链表，reverse 函数返回反转后子链表的头
            ListNode reversedList = reverse(lastGroup.next, nextGroup);

            // lastGroup 是上一个子链表的尾，其 next 指向当前反转子链表的头
            // 但是因为当前链表已经被反转，所以它指向的是反转后的链表的尾
            pointer = lastGroup.next;

            // 将上一个链表的尾连向反转后链表的头
            lastGroup.next = reversedList;

            // 当前反转后的链表的尾连向下一个子链表的头
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