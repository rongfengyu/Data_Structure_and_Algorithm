/*
101. �Գƶ�����
����һ����������������Ƿ��Ǿ���ԳƵġ�

���磬������ [1,2,2,3,4,4,3] �ǶԳƵġ�

    1
   / \
  2   2
 / \ / \
3  4 4  3
����������� [1,2,2,null,3,null,3] ���Ǿ���ԳƵ�:

    1
   / \
  2   2
   \   \
   3    3
˵��:

�����������õݹ�͵������ַ������������⣬��ܼӷ֡�


ע�⣺
1���ݹ鷨���Ƚ�ֱ�ۣ��ص������ݹ���ֹ������

2����������C++��ʹ�ö��У�C����

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

bool isSymmetricSub(struct TreeNode* pleft, struct TreeNode* pright��
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


//������
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