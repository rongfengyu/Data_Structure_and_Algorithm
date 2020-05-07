/*
408-��Ч������д

����һ���ǿ��ַ���?word?����д?abbr�������ַ����Ƿ���Ժ͸�������дƥ�䡣
����һ��?��word��?���ַ���������������Ч��д��

["word", "1ord", "w1rd", "wo1d", "wor1", "2rd", "w2d", "wo2", "1o1d", "1or1", "w1r1", "1o2", "2r1", "3d", "w3", "4"]

���� 1:
���� s = "internationalization", abbr = "i12iz4n":
���� true��

���� 2:
���� s = "apple", abbr = "a2e":
���� false��


����˼·��
? ? ��ָ��i,j�ֱ�ָ��s��abbr����j��ָλ��Ϊ���֣����������ǰ���ֵĴ�Сtimes��Ȼ��i����times���ַ���

���j��ָλ��Ϊ�ַ�����ֱ���ж���ָ����ָ�ַ��Ƿ����

����ж�i��j�Ƿ��Ѿ���ĩβ�����Ƿ���true������˵��ƥ�䲻�ϣ�����false

ע�⴦��ǰ��0��������ڱ�����ǰ��0�Ƿ�

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
            //��abbr�а�����ǰ��0������
            if(Character.isDigit(abbrs[j]) && abbrs[j] != '0'){
                int times = 0;//��ǰ���֣�eg:"i12i"�е�12
                int cnt = 1;
                while(j < abbrs.length && Character.isDigit(abbrs[j])){
                    times = cnt*times + abbrs[j] - '0';
                    cnt = 10*cnt;
                    j++;
                }
                i = i + times;
            }else{  //����ֱ�Ӱ�λ�ж��ַ�
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

