#include<bits/stdc++.h>
using namespace std;
//֮���ٸ�
// 0041->A 
//�򵥵�unicodeתascii���߼�������Ӧ�ý���Ӣ�� 
char UnicodeToAscii(int a, int b, int c, int d) {
	return d + (c << 4) + (b << 8) + (a << 12);
}
string parse_str() {
	string str_tmp;
	cin.get();
	char c;
	//���ܿ�����ܴ�֮��һ���ܲ����Ż�
	//	ԭ���� while ((cin.peek() != -1) && (c = cin.get()) != '\"') {
	while ( cin.peek() != -1) {
		c = cin.get();
		//if�������޳�switch���
		//��ͬ���ܵĿ��Ժϲ�
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
			} else if (c == 'f') {//formfeed ��bug 
				str_tmp += '\f';
			} else if (c == 'n') {
				str_tmp += '\n';
			} else if (c == 'r') {
				str_tmp += '\r';
			} else if (c == 't') {
				str_tmp += '\t';
			} else if (c == 'u') {
				
			}
			//��������� ��Ҫ����
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
	//cin.peek()�鿴��һ���ַ�������ʵ���ƶ�
	while ((c = cin.peek()) != -1) {

		if (c == '\"') {
			cout << parse_str();
		}
	}
	return 0;
}
