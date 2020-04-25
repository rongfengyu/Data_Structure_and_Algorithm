/*
771. 宝石与石头
 给定字符串J 代表石头中宝石的类型，和字符串 S代表你拥有的石头。 S 中每个字符代表了一种你拥有的石头的类型，你想知道你拥有的石头中有多少是宝石。

J 中的字母不重复，J 和 S中的所有字符都是字母。字母区分大小写，因此"a"和"A"是不同类型的石头。

示例 1:

输入: J = "aA", S = "aAAbbbb"
输出: 3
示例 2:

输入: J = "z", S = "ZZ"
输出: 0
注意:

S 和 J 最多含有50个字母。
 J 中的字符不重复。


题目解析


1、暴力求解，时间复杂度O(N^2)，空间复杂度：O(1)


2、使用hash，C语言相较于其他语言没有现成的，可以使用数组进行类似hash映射（J中字符不重复），J较短时，会浪费一些空间

时间复杂度：O(N)
空间复杂度：O(N)

*/


int numJewelsInStones(char* J, char* S) {
    int hash[52] = {0}, len_J = strlen(J), len_S = strlen(S), count = 0;
    
    // 字母包括小写字母和大写字母。
    // 将J中字母存入哈希表中。
    for (int i = 0; i < len_J; i++) {
        if (islower(J[i]))
            hash[J[i]-'a'] = 1;
        else 
            hash[J[i]-'A'+26] = 1;
    }
    // 判断石头字母是不是宝石。
    for (int i = 0; i < len_S; i++) {
        if ((islower(S[i]) && hash[S[i]-'a']) || (!islower(S[i]) && hash[S[i]-'A'+26]))
            count++;
    }
    
    return count;
}

