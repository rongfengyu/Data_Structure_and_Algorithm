/*
226. 翻转二叉树
翻转一棵二叉树。

示例：

输入：

     4
   /   \
  2     7
 / \   / \
1   3 6   9
输出：

     4
   /   \
  7     2
 / \   / \
9   6 3   1
备注:
这个问题是受到 Max Howell 的 原问题 启发的 ：

谷歌：我们90％的工程师使用您编写的软件(Homebrew)，但是您却无法在面试时在白板上写出翻转二叉树这道题，这太糟糕了。


思想



交换树中所有节点的左右子节点，即得到树的镜像。



求一棵树的镜像的过程：先前序遍历这棵树的每个节点，如果遍历到的节点有子节点，就交换它的左右两个子节点，当交换完所有非叶节点的左、右子节点之后，就得到了树的镜像。

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 105


struct TreeNode
{
        int val;
        TreeNode* left;
        TreeNode* right;
};

TreeNode *queue[N];
TreeNode *stack[N];


//创建树
void create_tree(TreeNode *&root)
{
        char c;
        scanf("%c", &c);
        if (c == '0')
        {
                root = NULL;
        }
        else
        {
            root = (TreeNode*)malloc(sizeof(TreeNode));
                root->val = c;
                create_tree(root->left);
                create_tree(root->right);
        }
}


//打印树
void print_tree(TreeNode *root)
{
        if (root != NULL)
        {
                printf("%c", root->val);
                print_tree(root->left);
                print_tree(root->right);
        }
}


//采用递归
void reverse_tree(TreeNode *root)
{
        if (root != NULL)
        {
                TreeNode *s;
                s = root->left;//反转左右子节点
                root->left = root->right;
                root->right = s;

                reverse_tree(root->left);
                reverse_tree(root->right);
        }
}


//采用队列
void reverse_tree_queue(TreeNode *root)
{
        TreeNode *temp, *p = root;
        int front, rear;
        if (root != NULL)
        {
                queue[0] = root;
                front = -1;
                rear = 0;
                while (front < rear)
                {
                        p = queue[++front];
                        temp = p->left;
                        p->left = p->right;
                        p->right = temp;
                        if (p->left != NULL)
                        {
                                queue[++rear] = p->left;
                        }
                        if (p->right != NULL)
                        {
                                queue[++rear] = p->right;
                        }
                }
        }
}

//采用栈
void reverse_tree_stack(TreeNode *root)
{
        int top = -1;
        TreeNode *p, *bt = root;
        if (root != NULL)
        {
                stack[++top] = root;
                while (top != -1)
                {
                        bt = stack[top--];
                        p = bt->right;
                        bt->right = bt->left;
                        bt->left = p;
                        if (bt->left)
                        {
                                stack[++top] = bt->left;
                        }
                        if (bt->right)
                        {
                                stack[++top] = bt->right;
                        }
                }
        }

}

int main(int argc, char* agrv[])
{
        TreeNode *bt;
        create_tree(bt);
        print_tree(bt);
        printf("\n");
        reverse_tree(bt);
        print_tree(bt);
        printf("\n");
        reverse_tree_queue(bt);
        print_tree(bt);
        printf("\n");
        reverse_tree_stack(bt);
        print_tree(bt);
        printf("\n");

        return 0;
}


//LeetCode
    public TreeNode invertTree(TreeNode root) {
        if (root == null) {//一定要检查指针为空
            return null;
        }
        root.left = invertTree(root.left);
        root.right = invertTree(root.right);

        TreeNode tmp = root.left;
        root.left = root.right;
        root.right = tmp;
        return root;
    }