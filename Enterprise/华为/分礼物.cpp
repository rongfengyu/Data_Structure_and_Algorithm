/*

https://blog.csdn.net/qq_40028201/article/details/90034926

题目描述
圣诞节到了，城堡里有k个小朋友，圣诞老人口袋里带了n件无差别的小礼物，请帮圣诞老人处理：将n个无差别的礼物分给k个小朋友的分法问题

给定n和k，数出分法种数，并枚举所有的分法

输出描述
仅一行，包含两个证书n(0 <= n <= 10)和k（0<= k<= 10), n 表示礼物的数量，k表示小朋友的数量

测试案例
输入：3 2

输出：
4
***|
**|*
*|**
|***


*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

void dfs(int index, int n, int k, int a, string path, vector<string>& res) {
    if (index == a) {
        res.push_back(path);
        return;
    }
    if (n > 0) {
        dfs(index + 1, n - 1, k, a, path + "*", res);
    }
    if (k > 0) {
        dfs(index + 1, n, k - 1, a, path + "|", res);
    }
}

int main(int argc, char *argv[]) {
    int n, k;
    cin >> n >> k;
    int total = 0;
    vector<string> res;

    int a = n + k - 1;
    dfs(0, n, k - 1, a, "", res);
    cout << res.size() <<endl;
    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << endl;
    }
    return 0;
}


【扩展】
排列组合 "n个球放入m个盒子m"问题 总结
https://blog.csdn.net/qwb492859377/article/details/50654627

测试：

echo "5 3" | ./test_flw

/*


21
*****||
****|*|
****||*
***|**|
***|*|*
***||**
**|***|
**|**|*
**|*|**
**||***
*|****|
*|***|*
*|**|**
*|*|***
*||****
|*****|
|****|*
|***|**
|**|***
|*|****
||*****


*/