/*
538. �Ѷ���������ת��Ϊ�ۼ���
�������� ���� ���ĸ��ڵ㣬�����Ľڵ�ֵ������ͬ�����㽫��ת��Ϊ�ۼ�����Greater Sum Tree����ʹÿ���ڵ� node ����ֵ����ԭ���д��ڻ���� node.val ��ֵ֮�͡�

����һ�£�������������������Լ��������

�ڵ���������������� С�� �ڵ���Ľڵ㡣
�ڵ���������������� ���� �ڵ���Ľڵ㡣
��������Ҳ�����Ƕ�����������
ע�⣺����� 1038: https://leetcode-cn.com/problems/binary-search-tree-to-greater-sum-tree/ ��ͬ

ʾ�� 1��

���룺[4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
�����[30,36,21,36,35,26,15,null,null,null,33,null,null,null,8]
ʾ�� 2��

���룺root = [0,null,1]
�����[1,null,1]
ʾ�� 3��

���룺root = [1,0,2]
�����[3,3,2]
ʾ�� 4��

���룺root = [3,2,4,1]
�����[7,9,4,10]
 

��ʾ��

���еĽڵ������� 0 �� 104 ֮�䡣
ÿ���ڵ��ֵ���� -104 �� 104 ֮�䡣
���е�����ֵ ������ͬ ��
��������Ϊ������������

˼��

  ����BST������������Ϊ���������

*/

int convertBST(Treenode root){
	//����BST����������������
	traverse(root);
	
	return root;
}

//��¼��ǰԪ������
int sum = 0;

void traverse(Treenode root){
	if(root == NULL){
		return ;
	}
	
	//�����������λ�ã�������䣬��ɽ���
	traverse(root.right);
	
	rank++;
	
	//ά���ۼӺ�
	sum += root.val;
	//����ڵ�value, BST��Ϊ�ۼ���
	root.val = sum;
	
	traverse(root.left);
}
