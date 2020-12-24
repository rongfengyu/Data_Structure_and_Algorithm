public boolean kmp(String resource, String target) {
    int len = target.length();
    if (len < 1) return false;
    //确定最长公共前后缀表
    int[] preTable = new int[len];
    commonPrefix(target,preTable);
    int i = 0,j = 0;//i指向resource  j指向target
    while (i < resource.length() && j < target.length()) {
        //如果两个指针指向的字符不同
        if (resource.charAt(i) != target.charAt(j)) {
            //根据最长公共前后缀表中j所在位置的值移动指针
            //如果小于0，resource中的指针后移，j不动
            if (preTable[j] < 0) {
                i++;
            } else {
                //否则将指针j的位置修改为最长公共前后缀表中j位置对应的值（重新确定匹配位置）
                j = preTable[j];
            }
        } else {
            i++;
            j++;
        }
    }
    //如果指针j的长度等于target长度说明匹配成功，否则，失败
    return j == target.length();
}
//获取target字符串的最长公共前后缀表
private void commonPrefix(String target, int[] preTable) {
    int n = target.length() - 1;
    if (n > 0 && n < 2) {
        preTable[0] = -1;
        return;
    }
    //为了编码方便
    preTable[0] = -1;
    //一个字符的最长公共前后缀长度一定是0
    preTable[1] =  0;
    int index = 2;
    int j = 1;
    while (j < n) {
        if (target.charAt(preTable[index - 1]) == target.charAt(j)) {
            preTable[index] = preTable[index - 1] + 1;
        } else {
            preTable[index] = 0;
        }
        index++;
        j++;
    }
}