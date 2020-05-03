/*
290. ���ʹ���
����һ�ֹ��� pattern ��һ���ַ��� str ���ж� str �Ƿ���ѭ��ͬ�Ĺ��ɡ�

����� ��ѭ ָ��ȫƥ�䣬���磬 pattern ���ÿ����ĸ���ַ��� str �е�ÿ���ǿյ���֮�������˫�����ӵĶ�Ӧ���ɡ�

ʾ��1:

����: pattern = "abba", str = "dog cat cat dog"
���: true
ʾ�� 2:

����:pattern = "abba", str = "dog cat cat fish"
���: false
ʾ�� 3:

����: pattern = "aaaa", str = "dog cat cat dog"
���: false
ʾ�� 4:

����: pattern = "abba", str = "dog dog dog dog"
���: false
*/

bool wordPattern(char* pattern, char* str) {
    int len = (strlen(str)/2)+2, size = 0;
    char** arr = malloc(sizeof(char*)*len);
    
    for (int i = 0; i < len; i++)
        arr[i] = malloc(sizeof(char)*15);
    
    arr[size] = strtok(str, " ");
    len = strlen(pattern);
    
    // �Կո�Ϊ�ָ���ָ�str�ַ�����
    while (arr[size] != NULL) {
        size++;
        arr[size] = strtok(NULL, " ");
    
        // ���ַ����������ַ��������ȣ���ƥ�䡣
        if (arr[size] == NULL && size != len)
            return false;
    }
    
    for (int i = 0; i < len; i++) {
        for (int j = i+1; j < len; j++) {
            /*
             ���������������false:
             1.pattern�е���ͬ�ַ�ӳ����str�еĲ�ͬ�ַ�����
             2.str�е���ͬ�ַ���ӳ����pattern�еĲ�ͬ�ַ���
             */
            if ((pattern[i] == pattern[j] && strcmp(arr[i], arr[j]) != 0) || (pattern[i] != pattern[j] && strcmp(arr[i], arr[j]) == 0))
                    return false;
        }
    }

    return true;
}
