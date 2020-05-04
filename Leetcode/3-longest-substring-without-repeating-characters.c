/*
3. ���ظ��ַ�����Ӵ�
����һ���ַ����������ҳ����в������ظ��ַ��� ��Ӵ� �ĳ��ȡ�

ʾ�� 1:

����: "abcabcbb"
���: 3 
����: ��Ϊ���ظ��ַ�����Ӵ��� "abc"�������䳤��Ϊ 3��
ʾ�� 2:

����: "bbbbb"
���: 1
����: ��Ϊ���ظ��ַ�����Ӵ��� "b"�������䳤��Ϊ 1��
ʾ�� 3:

����: "pwwkew"
���: 3
����: ��Ϊ���ظ��ַ�����Ӵ��� "wke"�������䳤��Ϊ 3��
     ��ע�⣬��Ĵ𰸱����� �Ӵ� �ĳ��ȣ�"pwke" ��һ�������У������Ӵ���

ע�⣺
����˼�룬��󴰿ڡ�����ָ��i,j���hash��

��1����ָ��j����Ԫ�ز��ظ�������max������ָ��jԪ����hash���еı��Ϊ���֣�����j

��2����ָ��j����Ԫ���ظ�����ָ����ƣ���ʱ����ָ��iԪ����hash���еı���������ʱ����������˭�ظ����ظ�Ԫ��ǰ��Ԫ�ض�Ҫ�������

�����ظ�����2����ֱ��i��j�����ַ���ĩβ��

ʱ�临�Ӷ�o(n)
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max(a,b) ((a)>(b)?(a):(b))

int lengthOfLongestSubstring(char *s, int len) {
    int ans = 0;
    int index[128]; // current index of character
    // try to extend the range [i, j]
    for (int j = 0, i = 0; j < len; j++) {
        i = max(index[s[j]], i);//��ָ���ƶ����ַ��ϴγ��ֵ�λ��
        ans = max(ans, j - i + 1);
        index[s[j]] = j + 1;//��¼�ַ����γ��ֵ�λ��
		printf("i=%d,j=%d,ans=%d\n",i,j,ans);
        printf("index['a']=%d,index['b']=%d,index['c']=%d\n",
              index['a'],index['b'],index['c']);
    }
    return ans; 
}

int main()
{
    char *str = "bbcac";
    int name2 = lengthOfLongestSubstring(str, strlen(str));
    printf("%d\n",name2);
    return 0;
}