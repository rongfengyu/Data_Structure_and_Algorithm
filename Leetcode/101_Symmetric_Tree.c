/*
101. 对称二叉树
给定一个二叉树，检查它是否是镜像对称的。

例如，二叉树 [1,2,2,3,4,4,3] 是对称的。

    1
   / \
  2   2
 / \ / \
3  4 4  3
但是下面这个 [1,2,2,null,3,null,3] 则不是镜像对称的:

    1
   / \
  2   2
   \   \
   3    3
说明:

如果你可以运用递归和迭代两种方法解决这个问题，会很加分。


注解：
1、递归法：比较直观，重点是理解递归终止条件。

2、迭代法：C++可使用队列，C语言

*/

#include <stdio.h>
	
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */


bool isSymmetric(struct TreeNode* root)
{
    if(root == NULL)
        return true;

    return isSymmetricSub(root->left, root->right);
}

bool isSymmetricSub(struct TreeNode* pleft, struct TreeNode* pright）
{
    if(pleft==NULL&&pright==NULL)

            return true;

    if(pleft==NULL||pright==NULL)

            return false;

    if(pleft->val!=pright->val)

            return false;


    return isSymmetricaSub(pleft->left,pright->right)&&isSymmetrical(pleft->right,pright->left);
}

int main(int argc, char* argv[])
{
    int nums[] = {1,2,2,3,4,4,3};
    int size = sizeof(nums)/sizeof(int);

    //create tree

    //isSymmetric?

    return 0;
}


//迭代法
    bool isSymmetric(TreeNode *root) {
        if (!root) return true;
        queue<TreeNode*> q1, q2;
        q1.push(root->left);
        q2.push(root->right);
        
        while (!q1.empty() && !q2.empty()) {
            TreeNode *node1 = q1.front();
            TreeNode *node2 = q2.front();
            q1.pop();
            q2.pop();
            if((node1 && !node2) || (!node1 && node2)) return false;
            if (node1) {
                if (node1->val != node2->val) return false;
                q1.push(node1->left);
                q1.push(node1->right);
                q2.push(node2->right);
                q2.push(node2->left);
            }
        }
        return true;
    }