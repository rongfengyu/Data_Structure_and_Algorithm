/*
445. ������� II
�������� �ǿ� ���������������Ǹ��������������λλ������ʼλ�á����ǵ�ÿ���ڵ�ֻ�洢һλ���֡�����������ӻ᷵��һ���µ�����

����Լ���������� 0 ֮�⣬���������ֶ��������㿪ͷ��

���ף�

��������������޸ĸ���δ������仰˵���㲻�ܶ��б��еĽڵ���з�ת��


ʾ����

���룺(7 -> 2 -> 4 -> 3) + (5 -> 6 -> 4)
�����7 -> 8 -> 0 -> 7

ע�⣺

���������������ע���λ

˼·1������˼·ʹ��ջ

˼·2������������������Ӻϲ���������

˼·3���Ӹ�λ��ʼ��ӣ��н�λ����¸�λ

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

    }
};


//˼·3
//����λ��������ν���������λ��ӣ���һ��lastָ��ָ����һ��������9����λ���������ǰ��λ��Ӻ�����λ���ɽ�lastָ�����λ��һ��
//Ȼ��last�ڵ㵽��ǰ�ڵ�������ֵΪ9�Ľڵ�ȫ����Ϊ0��
//�����ǰ��λ����Ҵ����˽�λ��Ϊ9����lastָ��ָ��ǰ��λ������lastָ�벻�䣬Ȼ����һ����λ��

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int count = 0, temp;
        ListNode *head, *last;
        for(head = l1; head; head = head->next)
            count++;
        for(head = l2; head; head = head->next)
            count--;
        if(count < 0)                       //�����������ȣ���l1ָ������l2ָ���������l2��ֵ�ӵ�l1��
            swap(l1,l2);            
        last = head = new ListNode(0);      //�����׼�һ��ֵΪ0�Ľڵ���Ϊ��ʼ��last�ڵ㣬������ոýڵ�ֵ��Ϊ0��ɾ���ýڵ�
        head->next = l1;
        for(int i = abs(count); i != 0; i--){  //��������λ����
            if(l1->val != 9)
                last = l1;
            l1 = l1->next;
        }
        while(l1){
            temp = l1->val + l2->val;
            if(temp > 9){                   //���������λ�������last��l1֮��������λ��ֵ
                temp -= 10;                 //��λ��ǰ��λ���ֵΪ8���ʽ�lastָ��ָ��ǰ��λ
                last->val += 1;
                last = last->next;
                while(last != l1){
                    last->val = 0;
                    last = last->next;
                }
            }
            else if(temp != 9)             
                last = l1;
            l1->val = temp;
            l1 = l1->next;
            l2 = l2->next;
        }
        return head->val == 1 ? head : head->next;
    }
};
