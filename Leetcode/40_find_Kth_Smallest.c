/*
215����δ������������ҵ��� k ������Ԫ�ء���ע�⣬����Ҫ�ҵ������������ĵ� k ������Ԫ�أ������ǵ� k ����ͬ��Ԫ�ء�

ʾ�� 1:

����: [3,2,1,5,6,4] �� k = 2
���: 2
ʾ�� 2:

����: [3,2,3,1,2,4,5,5,6] �� k = 4
���: 3
˵��:

����Լ��� k ������Ч�ģ��� 1 �� k �� ����ĳ��ȡ�


ע�⣺
1���μ�leetcode215

*/

public int[] getLeastNumbers(int[] arr, int k) {
    if (k == 0) {
        return new int[0];
    }
    // ʹ��һ�����ѣ��󶥶ѣ�
    // Java �� PriorityQueue Ĭ����С���ѣ���� comparator ����ʹ��������
    Queue<Integer> heap = new PriorityQueue<>(k, (i1, i2) -> Integer.compare(i2, i1));

    for (int e : arr) {
        // ��ǰ����С�ڶѶ�Ԫ�زŻ����
        if (heap.isEmpty() || heap.size() < k || e < heap.peek()) {
            heap.offer(e);
        }
        if (heap.size() > k) {
            heap.poll(); // ɾ���Ѷ����Ԫ��
        }
    }

    // �����е�Ԫ�ش�������
    int[] res = new int[heap.size()];
    int j = 0;
    for (int e : heap) {
        res[j++] = e;
    }
    return res;
}

//ʱ�临�Ӷ�O(N*logK)���ռ临�Ӷ�O(K)