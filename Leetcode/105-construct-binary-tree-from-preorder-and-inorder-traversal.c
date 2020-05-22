/*
105. 从前序与中序遍历序列构造二叉树
根据一棵树的前序遍历与中序遍历构造二叉树。

注意:
你可以假设树中没有重复的元素。

例如，给出

前序遍历 preorder = [3,9,20,15,7]
中序遍历 inorder = [9,3,15,20,7]
返回如下的二叉树：

    3
   / \
  9  20
    /  \
   15   7


注解：
关键在于前序遍历和中序遍历的特性

前序遍历：根节点是首元素
中序遍历：根节点左侧的值是其左子树，右侧的值是其右子树
因此，我们首先要得到从前序序列中获取根节点，然后遍历中序序列，找到根节点的位置，以此直到其左子树和右子树的范围。

当我得到其左子树之后，事情就开始重复了，我们仍然需要根据前序序列中找到这颗左子树的根节点，然后再根据中序序列得到这颗左子树根节点的左右子树。。。。右子树同理。

因此实际上就是个回溯。

*/

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {
	//写法仿照 剑指offer 面试题7 重建二叉树

	public TreeNode buildTree(int[] preorder, int[] inorder) {
		if(preorder == null || inorder == null || preorder.length==0){
			return null;
		}
		return buildCore(preorder,0,preorder.length-1,inorder,0,inorder.length-1);
	}
	private TreeNode buildCore(int[] preorder,int preSt,int preEnd,int[] inorder,int inSt,int inEnd){
		//前序遍历第一个节点是根节点
		int rootValue = preorder[preSt];
		TreeNode root = new TreeNode(rootValue);

		//前序序列只有根节点
		if(preSt == preEnd){
			return root;
		}
		//遍历中序序列，找到根节点的位置
		int rootInorder = inSt;
		while(inorder[rootInorder]!=rootValue&&rootInorder<=inEnd){
			rootInorder++;
		}

		//左子树的长度
		int leftLength = rootInorder - inSt;
		//前序序列中左子树的最后一个节点
		int leftPreEnd = preSt + leftLength;

		//左子树非空
		if(leftLength>0){
			//重建左子树
			root.left = buildCore(preorder,preSt+1,leftPreEnd,inorder,inSt,inEnd);
		}
		//右子树非空
		if(leftLength < preEnd - preSt){
			//重建右子树
			root.right = buildCore(preorder,leftPreEnd +1,preEnd,inorder,rootInorder+1,inEnd);
		}
		return root;
	}
}