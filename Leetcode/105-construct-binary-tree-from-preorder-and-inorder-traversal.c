/*
105. ��ǰ��������������й��������
����һ������ǰ�������������������������

ע��:
����Լ�������û���ظ���Ԫ�ء�

���磬����

ǰ����� preorder = [3,9,20,15,7]
������� inorder = [9,3,15,20,7]
�������µĶ�������

    3
   / \
  9  20
    /  \
   15   7


ע�⣺
�ؼ�����ǰ��������������������

ǰ����������ڵ�����Ԫ��
������������ڵ�����ֵ�������������Ҳ��ֵ����������
��ˣ���������Ҫ�õ���ǰ�������л�ȡ���ڵ㣬Ȼ������������У��ҵ����ڵ��λ�ã��Դ�ֱ�������������������ķ�Χ��

���ҵõ���������֮������Ϳ�ʼ�ظ��ˣ�������Ȼ��Ҫ����ǰ���������ҵ�����������ĸ��ڵ㣬Ȼ���ٸ����������еõ�������������ڵ������������������������ͬ��

���ʵ���Ͼ��Ǹ����ݡ�

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
	//д������ ��ָoffer ������7 �ؽ�������

	public TreeNode buildTree(int[] preorder, int[] inorder) {
		if(preorder == null || inorder == null || preorder.length==0){
			return null;
		}
		return buildCore(preorder,0,preorder.length-1,inorder,0,inorder.length-1);
	}
	private TreeNode buildCore(int[] preorder,int preSt,int preEnd,int[] inorder,int inSt,int inEnd){
		//ǰ�������һ���ڵ��Ǹ��ڵ�
		int rootValue = preorder[preSt];
		TreeNode root = new TreeNode(rootValue);

		//ǰ������ֻ�и��ڵ�
		if(preSt == preEnd){
			return root;
		}
		//�����������У��ҵ����ڵ��λ��
		int rootInorder = inSt;
		while(inorder[rootInorder]!=rootValue&&rootInorder<=inEnd){
			rootInorder++;
		}

		//�������ĳ���
		int leftLength = rootInorder - inSt;
		//ǰ�������������������һ���ڵ�
		int leftPreEnd = preSt + leftLength;

		//�������ǿ�
		if(leftLength>0){
			//�ؽ�������
			root.left = buildCore(preorder,preSt+1,leftPreEnd,inorder,inSt,inEnd);
		}
		//�������ǿ�
		if(leftLength < preEnd - preSt){
			//�ؽ�������
			root.right = buildCore(preorder,leftPreEnd +1,preEnd,inorder,rootInorder+1,inEnd);
		}
		return root;
	}
}