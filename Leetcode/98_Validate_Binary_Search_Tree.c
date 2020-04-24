/*
226. 验证二叉搜索树

给定一个二叉树，判断其是否是一个有效的二叉搜索树（BST）。二叉搜索树需要满足以下特征：
结点的左子树只包含小于当前结点的值；
结点的右子树只包含大于当前结点的值；
所有左子树和右子树自身也是二叉搜索树。


思想


比较相邻结点



BST的中序遍历结果是递增的，可以通过这一性质遍历BST节点，比较相邻节点大小

【扩展】
三个子问题：「二叉树是否为 BST」、「二叉树的最小值」、「二叉树的最大值」。

通过遍历节点，使用一变量保存上一节点的值或者保存按需保存最大最小值

*/

TreeNode prev; // 全局变量：指向中序遍历的上一个结点
boolean valid;

public boolean isValidBST(TreeNode root) {
    valid = true;
    prev = null;
    traverse(root);
    return valid;
}

void traverse(TreeNode curr) {
    if (curr == null) {
        return;
    }

    traverse(curr.left);

    // 中序遍历的写法，把操作写在两个递归调用中间
    if (prev != null && prev.val >= curr.val) {
        // 如果中序遍历的相邻两个结点大小关系不对，则二叉搜索树不合法
        valid = false;
    }
    // 维护 prev 指针
    prev = curr;

    traverse(curr.right);
}