/*
295. ����������λ��
��λ���������б��м����������б�����ż������λ�������м���������ƽ��ֵ��

���磬

[2,3,4] ����λ���� 3

[2,3] ����λ���� (2 + 3) / 2 = 2.5

���һ��֧���������ֲ��������ݽṹ��

void addNum(int num) - �������������һ�����������ݽṹ�С�
double findMedian() - ����Ŀǰ����Ԫ�ص���λ����

ʾ����
addNum(1)
addNum(2)
findMedian() -> 1.5
addNum(3) 
findMedian() -> 2

����:
����������������������� 0 �� 100 ��Χ�ڣ��㽫����Ż�����㷨��
����������� 99% ���������� 0 �� 100 ��Χ�ڣ��㽫����Ż�����㷨��

ע�⣺
��Hard��
ʹ�������ѽṹ��ά�������м�ֵ���ɿ����ҵ�Ŀ��ֵ

����������һ�����������������ҳ���λ�����������������ֶ�̬�������������ݣ����ʹ������洢����ôÿ���½���һ�����ݶ���������Ļ���Ч�ʺܵ͡�

����̬������˵һ��ʹ�õ����ݽṹ��ջ�����С����������ѡ�

�����У�����ʹ�� �� �������ݽṹ��

���Ƚ����ݷ�Ϊ�����֣�λ�� �ϱ����� ������Ҫ�� �±���С�� �����ݶ�ҪС��

Ϊ�˱�֤������ƽ�����䵽�������У��ڶ�̬�Ĳ����Ĺ����������������ݵ���Ŀ֮��ܳ��� 1��

Ϊ�˱�֤ �����е��������ݶ�С����С���е����ݣ��ڲ��������У�����ӽ�ȥ��������Ҫ�Ⱥ����ѵ����ֵ������С���е���Сֵ���бȽϡ�

*/

class MedianFinder {
public:
    /** initialize your data structure here. */
    MedianFinder() {

    }
    
    void addNum(int num) {

    }
    
    double findMedian() {

    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */
 
 
 class MedianFinder {
    public PriorityQueue<Integer> minheap, maxheap;
    public MedianFinder() {
        //ά���ϴ��Ԫ�ص���С��
        maxheap = new PriorityQueue<Integer>(Collections.reverseOrder());
        //ά����СԪ�ص�����
        minheap = new PriorityQueue<Integer>();
    }

    // Adds a number into the data structure.
    public void addNum(int num) {
        maxheap.add(num);
        minheap.add(maxheap.poll());
        if (maxheap.size() < minheap.size()) {//����������Ϊ����ʱ�����ݸ�ƫ����maxheap
            maxheap.add(minheap.poll());
        }
    }

    // Returns the median of current data stream
    public double findMedian() {
        if (maxheap.size() == minheap.size()) {
            return (maxheap.peek() + minheap.peek()) * 0.5;
        } else {
            return maxheap.peek();
        }
    }
};
 
 
 
