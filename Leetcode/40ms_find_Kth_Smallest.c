/*
������40. ��С��k����
������������ arr ���ҳ�������С�� k ���������磬����4��5��1��6��2��7��3��8��8�����֣�����С��4��������1��2��3��4��

ʾ�� 1��

���룺arr = [3,2,1], k = 2
�����[1,2] ���� [2,1]
ʾ�� 2��

���룺arr = [0,1,2,1], k = 1
�����[0]
 

���ƣ�

0 <= k <= arr.length <= 10000
0 <= arr[i] <= 10000


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