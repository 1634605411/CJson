#include<bits/stdc++.h>
using namespace std;
//之后再改
// 0041->A 
//简单的unicode转ascii码逻辑，但是应该仅限英语 
char UnicodeToAscii(int a, int b, int c, int d) {
	return d + (c << 4) + (b << 8) + (a << 12);
}
string parse_str() {
	string str_tmp;
	cin.get();
	char c;
	//可能开销会很大，之后看一下能不能优化
	//	原本是 while ((cin.peek() != -1) && (c = cin.get()) != '\"') {
	while ( cin.peek() != -1) {
		c = cin.get();
		//if语句可以修成switch语句
		//相同功能的可以合并
		if (c == '\\') {
			c = cin.get();
			if (c == '\"') {
				str_tmp += c;
			} else if (c == '\\') {
				str_tmp += c;
			} else if (c == '/') {
				str_tmp += c;
			} else if (c == 'b') {
				str_tmp += '\b';
			} else if (c == 'f') {//formfeed 有bug 
				str_tmp += '\f';
			} else if (c == 'n') {
				str_tmp += '\n';
			} else if (c == 'r') {
				str_tmp += '\r';
			} else if (c == 't') {
				str_tmp += '\t';
			} else if (c == 'u') {
				
			}
			//如果是其他 还要报错
		} else if (c == '\"') {
			return str_tmp;
		} else {
			str_tmp += c;
		}
	}
	//return str_tmp;
}
int main() {
	freopen("test.txt", "r", stdin);
	char c;
	//cin.peek()查看后一个字符，但是实际推动
	while ((c = cin.peek()) != -1) {

		if (c == '\"') {
			cout << parse_str();
		}
	}
	return 0;
}
