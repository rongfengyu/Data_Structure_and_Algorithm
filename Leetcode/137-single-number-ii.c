/*
137. ֻ����һ�ε����� II
����һ���ǿ��������飬����ĳ��Ԫ��ֻ����һ�����⣬����ÿ��Ԫ�ؾ����������Ρ��ҳ��Ǹ�ֻ������һ�ε�Ԫ�ء�

˵����

����㷨Ӧ�þ�������ʱ�临�Ӷȡ� ����Բ�ʹ�ö���ռ���ʵ����

ʾ�� 1:

����: [2,2,3,2]
���: 3
ʾ�� 2:

����: [0,1,0,1,0,1,99]
���: 99

ע�⣺
����λ���� ���� ��������������

*/
//�ⷨһ����ͳ��bit��������N�γ���
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int bitNum[32]={0};
        int res=0,len=nums.size();
        for(int i=0;i<32;i++){
            for(int j=0;j<len;j++){
                bitNum[i] +=(nums[j]>>i)&1;
            }
            res |= (bitNum[i]%3)<<i; //ͨ�ÿ�ܣ�3���Ի���k
        }
        return res;
    }
};

//�ⷨ�����������ƣ�N=3
class Solution {
public:
    int singleNumber(int A[], int n) {
        int ones = 0;
        int twos = 0;
        int threes;
        for (int i=0; i<n;i++){
            int t = A[i];
            twos |= ones&t;
            ones ^= t;
            threes = ones & twos; //Ĭ��threes��0;
            ones &= ~threes;
            twos &= ~threes;
        }
        return ones;
    }
};