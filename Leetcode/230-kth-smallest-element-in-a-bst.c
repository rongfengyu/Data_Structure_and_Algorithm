/*
230. �����������е�KС��Ԫ��
����һ����������������дһ������ kthSmallest ���������е� k ����С��Ԫ�ء�

˵����
����Լ��� k ������Ч�ģ�1 �� k �� ����������Ԫ�ظ�����

ʾ�� 1:

����: root = [3,1,4,null,2], k = 1
   3
  / \
 1   4
  \
   2
���: 1
ʾ�� 2:

����: root = [5,3,6,2,4,null,null,1], k = 3
       5
      / \
     3   6
    / \
   2   4
  /
 1
���: 3
���ף�
��������������������޸ģ�����/ɾ����������������ҪƵ���ز��ҵ� k С��ֵ���㽫����Ż� kthSmallest ������


˼��

  ����BST������������Ϊ���������

*/

int kthSmallest(Treenode root, int k){
	//����BST������������Ϊ���������
	traverse(root, k);
	return res;
}

// ��¼���
int res = 0;

//��¼��ǰԪ������
int rank = 0;

void traverse(Treenode root, int k){
	if(root == NULL){
		return ;
	}
	
	//�����������λ��
	traverse(root, left, k);
	
	rank++;
	
	if(rank == k){
		res = root.val;
		return ;
	}
	
	traverse(root, right, k);
}

��չ����TreeNode����ӳ�Աsize����¼�Դ˽ڵ�Ϊ���Ľڵ���������ӿ�����ٶȣ����ǻ�����һЩ��ɾԪ�ص�ά���ɱ�




