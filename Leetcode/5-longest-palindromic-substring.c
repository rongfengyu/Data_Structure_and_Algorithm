/*
5. ������Ӵ�
����һ���ַ��� s���ҵ� s ����Ļ����Ӵ�������Լ��� s ����󳤶�Ϊ 1000��

ʾ�� 1��

����: "babad"
���: "bab"
ע��: "aba" Ҳ��һ����Ч�𰸡�
ʾ�� 2��

����: "cbbd"
���: "bb"

ע�⣺

һ
Ѱ�һ��Ĵ����������˼���ǣ����м俪ʼ��������ɢ���жϻ��Ĵ���

for 0 <= i < len(s):
    �ҵ��� s[i] Ϊ���ĵĻ��Ĵ�
    �ҵ��� s[i] �� s[i+1] Ϊ���ĵĻ��Ĵ�
    ���´�

ʱ�临�Ӷ� O(N^2)���ռ临�Ӷ� O(1)��

��  DP��̬�滮

ʱ�临�Ӷ�һ�������ǿռ临�Ӷ�����Ҫ O(N^2) ���洢 DP table������������еĶ�̬�滮�����Žⷨ�����⡣

��  Manacher's Algorithm���������㷨��

ʱ�临�Ӷ�ֻ��Ҫ O(N)�������ýⷨ�Ƚϸ��ӣ��Ҹ�����Ϊû��Ҫ���ա�

*/


string palindrome(string& s, int l, int r)
{
    //��ֹ����Խ��
    while(l >= 0 && r < s.size() && s[l] == s[r])
    {
        //������չ��
        l--;r--;
    }
    //������s[l]��s[r]Ϊ���ĵ�����Ĵ�
    return s.substr(l+1, r-l-1);
}

string longestPalindrome(string s)
{
    string res;
    for(int i = 0; i < s.size(); i++)
    {
        //��s[i]Ϊ���ĵ�������Ӵ�
        string s1 = palindrome��s, i, i��
        //��s[i]��s[i+1]Ϊ���ĵ�������Ӵ�
        string s2 = palindrome��s, i, i+1��

        res = res.size() > s1.size() ? res : s1;
        res = res.size() > s2.size() ? res : s2;
    }
    
    return res;
}