/*
226. ��ת������
��תһ�ö�������

ʾ����

���룺

     4
   /   \
  2     7
 / \   / \
1   3 6   9
�����

     4
   /   \
  7     2
 / \   / \
9   6 3   1
��ע:
����������ܵ� Max Howell �� ԭ���� ������ ��

�ȸ裺����90���Ĺ���ʦʹ������д�����(Homebrew)��������ȴ�޷�������ʱ�ڰװ���д����ת����������⣬��̫����ˡ�


˼��



�����������нڵ�������ӽڵ㣬���õ����ľ���



��һ�����ľ���Ĺ��̣���ǰ������������ÿ���ڵ㣬����������Ľڵ����ӽڵ㣬�ͽ����������������ӽڵ㣬�����������з�Ҷ�ڵ�������ӽڵ�֮�󣬾͵õ������ľ���

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


//������
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


//��ӡ��
void print_tree(TreeNode *root)
{
        if (root != NULL)
        {
                printf("%c", root->val);
                print_tree(root->left);
                print_tree(root->right);
        }
}


//���õݹ�
void reverse_tree(TreeNode *root)
{
        if (root != NULL)
        {
                TreeNode *s;
                s = root->left;//��ת�����ӽڵ�
                root->left = root->right;
                root->right = s;

                reverse_tree(root->left);
                reverse_tree(root->right);
        }
}


//���ö���
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

//����ջ
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
        if (root == null) {//һ��Ҫ���ָ��Ϊ��
            return null;
        }
        root.left = invertTree(root.left);
        root.right = invertTree(root.right);

        TreeNode tmp = root.left;
        root.left = root.right;
        root.right = tmp;
        return root;
    }