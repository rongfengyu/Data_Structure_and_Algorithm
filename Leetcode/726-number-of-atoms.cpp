/*
726. 原子的数量
给定一个化学式formula（作为字符串），返回每种原子的数量。

原子总是以一个大写字母开始，接着跟随0个或任意个小写字母，表示原子的名字。

如果数量大于 1，原子后会跟着数字表示原子的数量。如果数量等于 1 则不会跟数字。例如，H2O 和 H2O2 是可行的，但 H1O2 这个表达是不可行的。

两个化学式连在一起是新的化学式。例如 H2O2He3Mg4 也是化学式。

一个括号中的化学式和数字（可选择性添加）也是化学式。例如 (H2O2) 和 (H2O2)3 是化学式。

给定一个化学式，输出所有原子的数量。格式为：第一个（按字典序）原子的名子，跟着它的数量（如果数量大于 1），然后是第二个原子的名字（按字典序），跟着它的数量（如果数量大于 1），以此类推。

示例 1:

输入: 
formula = "H2O"
输出: "H2O"
解释: 
原子的数量是 {'H': 2, 'O': 1}。
示例 2:

输入: 
formula = "Mg(OH)2"
输出: "H2MgO2"
解释: 
原子的数量是 {'H': 2, 'Mg': 1, 'O': 2}。
示例 3:

输入: 
formula = "K4(ON(SO3)2)2"
输出: "K4N2O14S4"
解释: 
原子的数量是 {'K': 4, 'N': 2, 'O': 14, 'S': 4}。
注意:

所有原子的第一个字母为大写，剩余字母都是小写。
formula的长度在[1, 1000]之间。
formula只包含字母、数字和圆括号，并且题目中给定的是合法的化学式。



dfs函数记录当前的倍数和子表达式
主要是原子的判断和括号的判断
原子一定是大写字母开头然后后面都是小写字母
左括号一定要找到当前能匹配到的最远的括号

*/


class Solution {
public:
	map<string, int> table;
	inline bool isBigAtom(const char & c) {
		return 'A' <= c && c <= 'Z';
	}
	inline bool isSmallAtom(const char & c) {
		return 'a' <= c && c <= 'z';
	}
	inline bool isLeftParentheses(const char & c) {
		return '(' == c;
	}
	inline bool isRightParentheses(const char & c) {
		return ')' == c;
	}
	inline bool isDigit(const char & c) {
		return '0' <= c && c <= '9';
	}
	void dfs(const string & formula, const int & multiple) {
		int nLength = formula.length();
		int i = 0;
		while (i < nLength) {
			if (isBigAtom(formula[i])) {
				string curAtom;
				curAtom.push_back(formula[i++]);
				while (isSmallAtom(formula[i])) {
					curAtom += formula[i];
					++i;
				}
				if (isDigit(formula[i])) {
					int count = 0;
					while (isDigit(formula[i])) {
						count = (count << 3) + (count << 1) + (formula[i] ^ 0x30);
						++i;
					}
					table[curAtom] += count * multiple;
				}
				else {
					table[curAtom] += multiple;
				}
			}
			else if (isLeftParentheses(formula[i])) {
				int count = 1;
				for (int j = i + 1; j < nLength; ++j) {
					if (isLeftParentheses(formula[j])) {
						++count;
					}
					else if (isRightParentheses(formula[j])) {
						--count;
					}
					if (0 == count) {
						string newFormula = formula.substr(i + 1, j - (i + 1));
						++j;
						if (isDigit(formula[j])) {
							int newMultiple = 0;
							while (isDigit(formula[j])) {
								newMultiple = (newMultiple << 3) + (newMultiple << 1) + (formula[j] ^ 0x30);
								++j;
							}
							dfs(newFormula, multiple * newMultiple);
						}
						else {
							dfs(newFormula, multiple);
						}
						i = j;
						break;
					}
				}
			}
		}
	}
	string countOfAtoms(string formula) {
		dfs(formula, 1);
		string ans;
		for (map<string, int>::iterator iter = table.begin(); iter != table.end(); iter++) {
			ans += iter->first;
			if (1 == iter->second) {
				continue;
			}
			ans += to_string(iter->second);
		}
		return ans;
	}
};



class Solution {
public:
    bool isUpper(char c) { return c >= 'A' && c <= 'Z';}
    bool isLower(char c) { return c >= 'a' && c <= 'z';}
    bool isDigit(char c) { return c >= '0' && c <= '9';}
    map<string, int> parse(const string exp, int l, int r) {
        string atom;
        int count = 0;
        map<string, int> m;
        int i = l;
        while (i <= r) {
            char c = exp[i];
            if (isUpper(c)) {
                if (!atom.empty()) {
                    m[atom] += max(count, 1);
                    atom.clear();
                    count = 0;
                }
                atom += c;
            } else if (isLower(c)) {
                atom += c;
            } else if (isDigit(c)) {
                count *= 10;
                count += c - '0';
            } else if (c == '(') {
                if (!atom.empty()) {
                    m[atom] += max(count, 1);
                    atom.clear();
                    count = 0;
                }
                int tl = ++i;
                int bucket = 1;
                while (i <= r && bucket != 0) {
                    if (exp[i] == '(') ++bucket;
                    if (exp[i] == ')') --bucket;
                    if (bucket == 0) break;
                    ++i;
                }
                auto m1 = parse(exp, tl, i - 1);
                count = 0;
                while (i + 1 <= r && isDigit(exp[i + 1])) {
                    count *= 10;
                    count += exp[++i] - '0';
                }
                count = max(count, 1);
                for (auto& p : m1) {
                    m[p.first] += p.second * count;
                }
                count = 0;
            }
            ++i;
        }
        if (!atom.empty()) m[atom] += max(count, 1);
        return m;
    }
    string countOfAtoms(string formula) {
        auto m = parse(formula, 0, formula.size() - 1);
        string s;
        for (auto& p : m) {
            s += p.first;
            if (p.second > 1) {
                s += to_string(p.second);
            }
        }
        return s;
    }
};
