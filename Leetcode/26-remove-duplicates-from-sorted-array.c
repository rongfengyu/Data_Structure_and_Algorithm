/*
26. ɾ�����������е��ظ���
����һ���������飬����Ҫ�� ԭ�� ɾ���ظ����ֵ�Ԫ�أ�ʹ��ÿ��Ԫ��ֻ����һ�Σ������Ƴ���������³��ȡ�

��Ҫʹ�ö��������ռ䣬������� ԭ�� �޸��������� ����ʹ�� O(1) ����ռ����������ɡ�

ʾ�� 1:

�������� nums = [1,1,2], 

����Ӧ�÷����µĳ��� 2, ����ԭ���� nums ��ǰ����Ԫ�ر��޸�Ϊ 1, 2�� 

�㲻��Ҫ���������г����³��Ⱥ����Ԫ�ء�
ʾ�� 2:

���� nums = [0,0,1,1,1,2,2,3,3,4],

����Ӧ�÷����µĳ��� 5, ����ԭ���� nums ��ǰ���Ԫ�ر��޸�Ϊ 0, 1, 2, 3, 4��

�㲻��Ҫ���������г����³��Ⱥ����Ԫ�ء�
 

˵��:

Ϊʲô������ֵ��������������Ĵ���������?

��ע�⣬�����������ԡ����á���ʽ���ݵģ�����ζ���ں������޸�����������ڵ������ǿɼ��ġ�

����������ڲ���������:

// nums ���ԡ����á���ʽ���ݵġ�Ҳ����˵������ʵ�����κο���
int len = removeDuplicates(nums);

// �ں������޸�����������ڵ������ǿɼ��ġ�
// ������ĺ������صĳ���, �����ӡ�������иó��ȷ�Χ�ڵ�����Ԫ�ء�
for (int i = 0; i < len; i++) {
    print(nums[i]);
}

ע�⣺�������飬ԭ��ɾ��Ҫ��ʹ�ö����ڴ棬C���Ե�ʵ����Щ�������ָ���ζ��
*/

#include <stdio.h>
int removeDuplicates(int nums[], int len)
{
    if (len == 0) return 0;
    //�ж�������
    int number = 0;//��Ǽ���
    for (int i=0; i < len ; i++) {
        if ( nums[i] != nums[number] ) {
            number++;
            nums[number] = nums[i];
        }
    }
    number+=1; //���+1��Ϊ���ָ���
    
    for (int i=0; i < len ; i++) {
        printf("%d,", nums[i]);
    }
    return number;
}

int main(int argc, char* argv[])
{
    int num[] = {0,1,1,2,2,3,3,4};
    int len = sizeof(num)/sizeof(int);//��������ʵ��ʱ����Ϊָ�룩�ᶪʧ������Ϣ
    printf("num=%d", removeDuplicates(num, len));

    return 0;
}


int removeDuplicates(int[] nums) {
    if (nums.length == 0) {
        return 0;
    }
    int slow = 0, fast = 0;
    while (fast < nums.length) {
        if (nums[fast] != nums[slow]) {
            slow++;
            // ά�� nums[0..slow] ���ظ�
            nums[slow] = nums[fast];
        }
        fast++;
    }
    // ���鳤��Ϊ���� + 1
    return slow + 1;
}