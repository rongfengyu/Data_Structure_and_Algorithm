/*
290. 单词规律
给定一种规律 pattern 和一个字符串 str ，判断 str 是否遵循相同的规律。

这里的 遵循 指完全匹配，例如， pattern 里的每个字母和字符串 str 中的每个非空单词之间存在着双向连接的对应规律。

示例1:

输入: pattern = "abba", str = "dog cat cat dog"
输出: true
示例 2:

输入:pattern = "abba", str = "dog cat cat fish"
输出: false
示例 3:

输入: pattern = "aaaa", str = "dog cat cat dog"
输出: false
示例 4:

输入: pattern = "abba", str = "dog dog dog dog"
输出: false
*/

bool wordPattern(char* pattern, char* str) {
    int len = (strlen(str)/2)+2, size = 0;
    char** arr = malloc(sizeof(char*)*len);
    
    for (int i = 0; i < len; i++)
        arr[i] = malloc(sizeof(char)*15);
    
    arr[size] = strtok(str, " ");
    len = strlen(pattern);
    
    // 以空格为分割符分割str字符串。
    while (arr[size] != NULL) {
        size++;
        arr[size] = strtok(NULL, " ");
    
        // 若字符串个数与字符个数不等，则不匹配。
        if (arr[size] == NULL && size != len)
            return false;
    }
    
    for (int i = 0; i < len; i++) {
        for (int j = i+1; j < len; j++) {
            /*
             以下两种情况返回false:
             1.pattern中的相同字符映射了str中的不同字符串。
             2.str中的相同字符串映射了pattern中的不同字符。
             */
            if ((pattern[i] == pattern[j] && strcmp(arr[i], arr[j]) != 0) || (pattern[i] != pattern[j] && strcmp(arr[i], arr[j]) == 0))
                    return false;
        }
    }

    return true;
}
