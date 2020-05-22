/*
93. 复原IP地址
给定一个只包含数字的字符串，复原它并返回所有可能的 IP 地址格式。

有效的 IP 地址正好由四个整数（每个整数位于 0 到 255 之间组成），整数之间用 '.' 分隔。

示例:

输入: "25525511135"
输出: ["255.255.11.135", "255.255.111.35"]

注解:

DFS回溯算法

分析剪枝条件：

1、一开始，字符串的长度小于 4 或者大于 12 ，一定不能拼凑出合法的 ip 地址（这一点可以一般化到中间结点的判断中，以产生剪枝行为）；

2、每一个结点可以选择截取的方法只有 3 种：截 1 位、截 2 位、截 3 位，因此每一个结点可以生长出的分支最多只有 3 条分支；

根据截取出来的字符串判断是否是合理的 ip 段，这里写法比较多，可以先截取，再转换成 int ，再判断。我采用的做法是先转成 int，是合法的 ip 段数值以后，再截取。

3、由于 ip 段最多就 4 个段，因此这棵三叉树最多 4 层，这个条件作为递归终止条件之一；

4、每一个结点表示了求解这个问题的不同阶段，需要的状态变量有：

splitTimes：已经分割出多少个 ip 段；
begin：截取 ip 段的起始位置；
path：记录从根结点到叶子结点的一个路径（回溯算法常规变量，是一个栈）；
res：记录结果集的变量，常规变量。

总结：这个问题思想不难，但是细节比较繁琐，什么时候递归终止，如何手动截取字符串，再转换成 int 类型，还有如何在中间结点发现可以剪枝，这些细节需要在编码的时候考虑清楚。


*/

class Solution 
{
public:
	string s;
	int n;
	vector<string> results;
	vector<string> restoreIpAddresses(string s) 
	{
		this->s = s;
		this->n = s.size();
		dfs(0, 0, "");
		return results;
	}

	void dfs(int index, int depth, string path)
	{
		if (depth == 4)
		{
			path.pop_back();
			results.push_back(path);
			return;
		}

		int minI = max(index + 1, n - (3 - depth) * 3);
		int maxI = min(index + 3, n - (3 - depth));
		for (int i = minI; i <= maxI; i++)
		{
			string split = s.substr(index, i - index);
			int len = split.size();
			if (split.size() > 1 && split[0] == '0') break;
			if (stoi(split) <= 255)
			{
				dfs(i, depth + 1, path + (split + '.'));
			}
		}
	}
}; 