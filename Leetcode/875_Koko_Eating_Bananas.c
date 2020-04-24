/*
875. Koko Eating Bananas

��Ŀ����:

����ϲ�����㽶��������?N?���㽶���� i ������?piles[i]?���㽶�������Ѿ��뿪�ˣ�����?H?Сʱ�������


������Ծ��������㽶���ٶ�?K?����λ����/Сʱ����ÿ��Сʱ��������ѡ��һ���㽶�����гԵ� K �����������㽶���� K ���������Ե���ѵ������㽶��Ȼ����һСʱ�ڲ����ٳԸ�����㽶��??


����ϲ�������ԣ�����Ȼ���ھ�������ǰ�Ե����е��㽶��


������������ H Сʱ�ڳԵ������㽶����С�ٶ� K��K Ϊ��������

?



ʾ�� 1��



����: piles = [3,6,7,11], H = 8

���: 4


ʾ��?2��



����: piles = [30,11,23,4,20], H = 5

���: 30


ʾ��?3��



����: piles = [30,11,23,4,20], H = 6

���: 23
?



��ʾ��


1 <= piles.length <= 10^4

piles.length <= H <= 10^9

1 <= piles[i] <= 10^9




ע�⣺
1������ʽ��K��1��ʼ����ֱ���ҵ�����ֵ��Ч�ʲ��ѣ�ʹ�ö��ֲ��ң�����������Χ�ҵ�Ŀ��K��

2����ξ�����С��ʼ��K��Χ��K���ֵ���ᳬ������piles[i]��K��Сֵ����С��sum��piles[i]��/H�����Կ��ܵ�������

3����ȡH��ʱҪע������ȡ��
*/

#include <stdio.h>

int canEat(int* piles, int pilesSize, int speed, int H)
{
    int sum = 0;

    int i=0; 
    for(i=0; i< pilesSize; i++)
    {
        sum += (piles[i] + speed - 1) / speed;//����ȡ��

        if(sum > H) break;
    }
    return sum > H;
}

int minEatingSpeed(int* piles, int pilesSize, int H)
{
    int maxVal = 0;
	int sum = 0;
    int i=0; 
    for(i=0; i< pilesSize; i++)
    {
        maxVal = piles[i] > maxVal ? piles[i] : maxVal;
	sum += piles[i];
    }
	
    int left = 1;
    int count = 0;
    //int left = (sum + H - 1)/ H;//δ���ǿ��ܵ�������Ż���Сֵ
    int right = maxVal;
    while(left < right)
    {
        int mid = left + (right - left)/2;
        if(canEat(piles, pilesSize, mid, H))
	{
            left = mid + 1;
	    count++;
	}
        else
	{
            right = mid;
	    count++;
	}
    }

    printf("count=%d\n", count);
    return left;
}


int main(int argc, char* argv[])
{
    int piles[] = {3,6,7,11};
    int H = 8;
    int size = sizeof(piles)/sizeof(int);
    int K = minEatingSpeed(piles, size, H);
    printf("K=%d\n", K);

    return 0;
}