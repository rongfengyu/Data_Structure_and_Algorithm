/*
426. 转换BST为有序双向链表

同 面试题36. 二叉搜索树与双向链表（Medium）

给定一棵二叉搜索树，将其转换成一个排序的循环双向链表。链表不使用新的结点，而是用二叉树的结点，调整其中指针的指向（left 指针表示链表的 prev 指针，right 指针表示链表的 next 指针）。


思想



有序——BST中序遍历

这段代码有两个需要注意的地方：
在遍历二叉树的时候，我们是把结点串连成一个双向链表，但是题目要求返回的是双向循环链表，所以我们在最后把链表的头尾结点连接起来。
实际上我们只需要提前保存右子树的指针，因为中序遍历会先遍历左子树，再处理当前结点。如果是前序遍历的话，左右子树的指针都需要保存。


【扩展】
虽然本文的两道题使用的都是中序遍历序列，但这种方法对于前/中/后序遍历都适用，只需要调整「处理当前结点」和「递归调用两个子树」的顺序即可。
这里列出几个也用到该技巧的题目，各位小伙伴可以尝试练习一下：
LeetCode 114. Flatten Binary Tree to Linked List
LeetCode 116. Populating Next Right Pointers in Each Node
LeetCode 117. Populating Next Right Pointers in Each Node II

*/

Node head; // 链表的头结点
Node last; // 二叉树遍历中的前一个结点，也是链表的尾结点

public Node treeToDoublyList(Node root) {
    head = null;
    last = null;
    traverse(root);
    // 将双向链表转为双向循环链表
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
    // 提前保存右子树指针
    Node right = curr.right;
    // 中序遍历
    traverse(curr.left);
    // 将当前结点加入链表
    curr.left = null;
    curr.right = null;
    if (head == null) {
        head = curr;
    } else {
        curr.left = last;
        last.right = curr;
    }
    last = curr; // 更新 last 指针
    traverse(right);
}