/*
426. ת��BSTΪ����˫������

ͬ ������36. ������������˫������Medium��

����һ�ö���������������ת����һ�������ѭ��˫����������ʹ���µĽ�㣬�����ö������Ľ�㣬��������ָ���ָ��left ָ���ʾ����� prev ָ�룬right ָ���ʾ����� next ָ�룩��


˼��



���򡪡�BST�������

��δ�����������Ҫע��ĵط���
�ڱ�����������ʱ�������ǰѽ�㴮����һ��˫������������ĿҪ�󷵻ص���˫��ѭ�������������������������ͷβ�������������
ʵ��������ֻ��Ҫ��ǰ������������ָ�룬��Ϊ����������ȱ������������ٴ���ǰ��㡣�����ǰ������Ļ�������������ָ�붼��Ҫ���档


����չ��
��Ȼ���ĵ�������ʹ�õĶ�������������У������ַ�������ǰ/��/������������ã�ֻ��Ҫ����������ǰ��㡹�͡��ݹ����������������˳�򼴿ɡ�
�����г�����Ҳ�õ��ü��ɵ���Ŀ����λС�����Գ�����ϰһ�£�
LeetCode 114. Flatten Binary Tree to Linked List
LeetCode 116. Populating Next Right Pointers in Each Node
LeetCode 117. Populating Next Right Pointers in Each Node II

*/

Node head; // �����ͷ���
Node last; // �����������е�ǰһ����㣬Ҳ�������β���

public Node treeToDoublyList(Node root) {
    head = null;
    last = null;
    traverse(root);
    // ��˫������תΪ˫��ѭ������
    if (head != null) {
        head.left = last;
        last.right = head;
    }
    return head;
}

void traverse(Node curr) {
    if (curr == null) {
        return;
    }
    // ��ǰ����������ָ��
    Node right = curr.right;
    // �������
    traverse(curr.left);
    // ����ǰ����������
    curr.left = null;
    curr.right = null;
    if (head == null) {
        head = curr;
    } else {
        curr.left = last;
        last.right = curr;
    }
    last = curr; // ���� last ָ��
    traverse(right);
}