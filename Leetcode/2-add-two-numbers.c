/*
2. �������
�������� �ǿ� ������������ʾ�����Ǹ������������У����Ǹ��Ե�λ���ǰ��� ���� �ķ�ʽ�洢�ģ��������ǵ�ÿ���ڵ�ֻ�ܴ洢 һλ ���֡�

��������ǽ��������������������᷵��һ���µ���������ʾ���ǵĺ͡�

�����Լ���������� 0 ֮�⣬���������������� 0 ��ͷ��

ʾ����

���룺(2 -> 4 -> 3) + (5 -> 6 -> 4)
�����7 -> 0 -> 8
ԭ��342 + 465 = 807

ע�⣺

���������������ע���λ

ʱ�临�Ӷ�o(n)
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
    int c = 0; //�洢��λֵ
    struct ListNode *head, *cur, *next;
    head = (struct ListNode *)malloc(sizeof(struct ListNode));//�½�һ������ͷ��㣬���ڱ����������Ҫ�ͷ�ɾ���˽ڵ㣡
    head->next = NULL;
    cur = head;
    while (l1 || l2 || c) //һֱ����ֱ������������Ϊֹ����c��������Ӧ�����������ֻ��һ���ڵ�����
    {
        next = (struct ListNode *)malloc(sizeof(struct ListNode));
        next->next = NULL;
        cur->next = next;
        cur = next;
        l1 != NULL ? (c += l1->val, l1 = l1->next) : (c += 0); //��Ŀ�������ɧ�÷���������
        l2 != NULL ? (c += l2->val, l2 = l2->next) : (c += 0);
        cur->val = c % 10;  // ȡ��λֵ�ĸ�λ��
        c = c / 10; // ȡ��λֵ��ʮλ��
    }
    struct listNode *del = head;
    head = head ->next; // ˵ʵ������̫����ΪɶҪ��ô�ɣ��ѵ���Ӧ��ֱ�ӷ���ͷ�����? 
    free(del); 
    return head;
}
