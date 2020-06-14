/*
445. 两数相加 II
给你两个 非空 链表来代表两个非负整数。数字最高位位于链表开始位置。它们的每个节点只存储一位数字。将这两数相加会返回一个新的链表。

你可以假设除了数字 0 之外，这两个数字都不会以零开头。

进阶：

如果输入链表不能修改该如何处理？换句话说，你不能对列表中的节点进行翻转。


示例：

输入：(7 -> 2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 8 -> 0 -> 7

注解：

基本的链表操作，注意进位

思路1、常规思路使用栈

思路2、将两个链表逆序，相加合并，再逆序

思路3、从高位开始相加，有进位则更新高位

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


//思路3
//将数位对齐后，依次将两链表数位相加，用一个last指针指向上一个计算后非9的数位，即如果当前数位相加后发生进位，可将last指向的数位加一，
//然后last节点到当前节点间的所有值为9的节点全都变为0；
//如果当前数位相加且处理了进位后不为9，则last指针指向当前数位，否则last指针不变，然后下一个数位。

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int count = 0, temp;
        ListNode *head, *last;
        for(head = l1; head; head = head->next)
            count++;
        for(head = l2; head; head = head->next)
            count--;
        if(count < 0)                       //计算两链表长度，将l1指向长链，l2指向短链，将l2的值加到l1中
            swap(l1,l2);            
        last = head = new ListNode(0);      //在链首加一个值为0的节点作为初始的last节点，如果最终该节点值仍为0则删除该节点
        head->next = l1;
        for(int i = abs(count); i != 0; i--){  //将两链数位对齐
            if(l1->val != 9)
                last = l1;
            l1 = l1->next;
        }
        while(l1){
            temp = l1->val + l2->val;
            if(temp > 9){                   //如果发生进位，则更新last到l1之间所有数位的值
                temp -= 10;                 //进位后当前数位最大值为8，故将last指针指向当前数位
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
