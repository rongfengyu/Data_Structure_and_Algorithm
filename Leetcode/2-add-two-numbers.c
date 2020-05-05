/*
2. 两数相加
给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。

如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。

您可以假设除了数字 0 之外，这两个数都不会以 0 开头。

示例：

输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 0 -> 8
原因：342 + 465 = 807

注解：

基本的链表操作，注意进位

时间复杂度o(n)
*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){

}

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
    int c = 0; //存储进位值
    struct ListNode *head, *cur, *next;
    head = (struct ListNode *)malloc(sizeof(struct ListNode));//新建一个链表头结点，用于保存结果，最后要释放删除此节点！
    head->next = NULL;
    cur = head;
    while (l1 || l2 || c) //一直迭代直到两个链表都空为止。把c放这里是应对两个链表各只有一个节点的情况
    {
        next = (struct ListNode *)malloc(sizeof(struct ListNode));
        next->next = NULL;
        cur->next = next;
        cur = next;
        l1 != NULL ? (c += l1->val, l1 = l1->next) : (c += 0); //三目运算符的骚用法，记下了
        l2 != NULL ? (c += l2->val, l2 = l2->next) : (c += 0);
        cur->val = c % 10;  // 取进位值的个位数
        c = c / 10; // 取进位值的十位数
    }
    struct listNode *del = head;
    head = head ->next; // 说实话，不太明白为啥要这么干，难道不应该直接返回头结点吗? 
    free(del); 
    return head;
}
