/*
408-有效单词缩写

给定一个非空字符串?word?和缩写?abbr，返回字符串是否可以和给定的缩写匹配。
比如一个?“word”?的字符串仅包含以下有效缩写：

["word", "1ord", "w1rd", "wo1d", "wor1", "2rd", "w2d", "wo2", "1o1d", "1or1", "w1r1", "1o2", "2r1", "3d", "w3", "4"]

样例 1:
给出 s = "internationalization", abbr = "i12iz4n":
返回 true。

样例 2:
给出 s = "apple", abbr = "a2e":
返回 false。


解题思路：
? ? 用指针i,j分别指向s与abbr，若j所指位置为数字，则先求出当前数字的大小times，然后i跳过times个字符。

如果j所指位置为字符，则直接判断两指针所指字符是否相等

最后判断i，j是否都已经到末尾，若是返回true，否则说明匹配不上，返回false

注意处理前导0的情况，在本题中前导0非法

*/

public class Solution {
    /**
     * @param word: a non-empty string
     * @param abbr: an abbreviation
     * @return: true if string matches with the given abbr or false
     */
    public boolean validWordAbbreviation(String word, String abbr) {
        // write your code here
        char[] words = word.toCharArray();
        char[] abbrs = abbr.toCharArray();
        
        int i=0, j=0;
        while(i < words.length && j < abbrs.length){
            //若abbr中包含非前导0的数字
            if(Character.isDigit(abbrs[j]) && abbrs[j] != '0'){
                int times = 0;//当前数字，eg:"i12i"中的12
                int cnt = 1;
                while(j < abbrs.length && Character.isDigit(abbrs[j])){
                    times = cnt*times + abbrs[j] - '0';
                    cnt = 10*cnt;
                    j++;
                }
                i = i + times;
            }else{  //否则直接按位判断字符
                if(words[i] != abbrs[j])
                    return false;
                i++;
                j++;
            }
        }
        
        if(i == words.length && j == abbrs.length)
            return true;
        return false;
    }
}

