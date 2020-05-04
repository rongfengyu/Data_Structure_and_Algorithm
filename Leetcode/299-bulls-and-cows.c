/*
299. ��������Ϸ
�����ں���������� �����֣�Bulls and Cows����Ϸ����д��һ��������������Ѳ¡�
ÿ�����²�������һ����ʾ���������ж���λ���ֺ�ȷ��λ�ö��¶��ˣ���Ϊ��Bulls��, ��ţ����
�ж���λ���ֲ¶��˵���λ�ò��ԣ���Ϊ��Cows��, ��ţ����
������ѽ��������ʾ�����£�ֱ���³��������֡�

��д��һ�������������ֺ����ѵĲ²���������ʾ�ĺ������� A ��ʾ��ţ���� B ��ʾ��ţ��

��ע���������ֺ����ѵĲ²��������ܺ����ظ����֡�

ʾ�� 1:

����: secret = "1807", guess = "7810"

���: "1A3B"

����: 1 ��ţ�� 3 ��ţ����ţ�� 8����ţ�� 0, 1 �� 7��
ʾ�� 2:

����: secret = "1123", guess = "0111"

���: "1A1B"

����: ���Ѳ²����еĵ�һ�� 1 �ǹ�ţ���ڶ���������� 1 �ɱ���Ϊ��ţ��
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a,b) ((a)<(b)?(a):(b))

char *getHint(char *secret, char *guess, int len)
{
    char * name = NULL;
    name = (char *)malloc(4+1);
    
    int a = 0;
    int b = 0; 
    int mapS[10] = {0};
    int mapG[10] = {0};
    
    for(int i=0; i<len; i++)
    {
        if(secret[i] == guess[i])
            a++;
        else
        {
            mapS[secret[i]-'0']++;
            mapG[guess[i]-'0']++;
        }
    }
    for(int i=0; i<10; i++)
    {
        b += min(mapS[i], mapG[i]);
    }
    
    snprintf(name, 5, "%dA%dB", a, b);
    return name;
}

int main(int argc, char* argv[])
{
    char secret[] = "1807";
    char guess[] = "7810";
    
    char *result = NULL;
    result = getHint(secret, guess, strlen(secret));
    printf("the result is %s\n", result);
    
    free(result);
    result=NULL;
	return 0;
}
