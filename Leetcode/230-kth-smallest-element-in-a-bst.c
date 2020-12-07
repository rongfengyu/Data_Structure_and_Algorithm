/*
230. 二叉搜索树中第K小的元素
给定一个二叉搜索树，编写一个函数 kthSmallest 来查找其中第 k 个最小的元素。

说明：
你可以假设 k 总是有效的，1 ≤ k ≤ 二叉搜索树元素个数。

示例 1:

输入: root = [3,1,4,null,2], k = 1
   3
  / \
 1   4
  \
   2
输出: 1
示例 2:

输入: root = [5,3,6,2,4,null,null,1], k = 3
       5
      / \
     3   6
    / \
   2   4
  /
 1
输出: 3
进阶：
如果二叉搜索树经常被修改（插入/删除操作）并且你需要频繁地查找第 k 小的值，你将如何优化 kthSmallest 函数？


思想

  利用BST的中序遍历结果为升序的特性

*/

int kthSmallest(Treenode root, int k){
	//利用BST的中序遍历结果为升序的特性
	traverse(root, k);
	return res;
}

// 记录结果
int res = 0;

//记录当前元素排名
int rank = 0;

void traverse(Treenode root, int k){
	if(root == NULL){
		return ;
	}
	
	//中序遍历代码位置
	traverse(root, left, k);
	
	rank++;
	
	if(rank == k){
		res = root.val;
		return ;
	}
	
	traverse(root, right, k);
}

扩展：在TreeNode中添加成员size，记录以此节点为根的节点总数，会加快查找速度，但是会增加一些增删元素的维护成本




