#include<bits/stdc++.h>
using namespace std;
#define STR 0
#define NUM 1
#define OBJ 2
#define BOOL 3
#define ARR 4
//���е�value���ͱ��滻��pointer

//����ûд 
//arrayûд boolûд 

class pointer {
	public:
		int type;
		int pos;
};
class String {
	public:
		string str;
};
class Number {
	public:
		//��Ҫ�Ӹ���־
		int int_num;
		double dou_num;
};
class Object {
	public:
		vector<pair<pointer, pointer>>v;
} obj;
class Bool {
	public:
		int Bool;//true false null
};
class Array {
	public:
		vector<pointer>v;
};

vector<String>vec_String;
vector<Number>vec_Number;
vector<Object>vec_Object;
vector<Bool>vec_Bool;
vector<Array>vec_Array;



pointer parse_Object();


pointer parse_num() {
	string s;
	Number num;
	while (isdigit(cin.peek()) || cin.peek() == 'e' || cin.peek() == '-' || cin.peek() == '+')s.push_back(cin.get());
	if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'e')) {
		num.dou_num = stof(s);
		vec_Number.push_back(num);
		return {NUM, (int)vec_Number.size() - 1};
	} else {
		num.int_num = stof(s);
		vec_Number.push_back(num);
		return {NUM, (int)vec_Number.size() - 1};
	}
}


char UnicodeToAscii(char a[4]) {
	//֮���ٸ�
	// 0041->A
	//�򵥵�unicodeתascii���߼�������Ӧ�ý���Ӣ��
	for (int i = 0; i < 4; i++) {
		if (a[i] >= '0' && a[i] <= '9') {
			a[i] = a[i] - '0';
		} else {
			a[i] = a[i] - 'A' + 10;
		}
	}
	//�ǵ�����д���� ���Ϸ��ķ���
	//char�ķ�Χ��-128~127
	return a[3] + (a[2] << 4) + (a[1] << 8) + (a[0] << 12);
}
pointer parse_str() {
	String str_tmp;
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
				str_tmp.str += c;
			} else if (c == '\\') {
				str_tmp.str += c;
			} else if (c == '/') {
				str_tmp.str += c;
			} else if (c == 'b') {
				str_tmp.str += '\b';
			} else if (c == 'f') {//formfeed ��bug �����һ������
				str_tmp.str += '\f';
			} else if (c == 'n') {
				str_tmp.str += '\n';
			} else if (c == 'r') {
				str_tmp.str += '\r';
			} else if (c == 't') {
				str_tmp.str += '\t';
			} else if (c == 'u') {
				char a[4];//������һ��
				for (int i = 0; i < 4; i++) {
					a[i] = cin.get();
				}
				str_tmp.str += UnicodeToAscii(a);
			}
			//��������� ��Ҫ����
		} else if (c == '\"') {
			vec_String.push_back(str_tmp);
			return {STR, (int)vec_String.size() - 1};
		} else {
			str_tmp.str += c;
		}
	}
	//�ȼ�һ���˳��ĵط�
	vec_String.push_back(str_tmp);
	return {STR, (int)vec_String.size() - 1};
}
void parse_whitespace() {
	//���кܶ������ ����ո� ���е� ֮���ٿ�һ��json�ļ�
	//������if
	while (cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t') {
		cin.get();
	}
}
pointer parse_arr() {//��ûд
	Array a;
	vec_Array.push_back(a);
	return {ARR, (int)vec_Array.size() - 1};
};
pointer parse_bool() {//��ûд
	Bool b;
	b.Bool = 1;
	vec_Bool.push_back(b);
	return {BOOL, (int)vec_Bool.size() - 1};
};
pointer parse_value() {
	parse_whitespace();
	char c;
	//cout << "YES" << endl;
	while ((c = cin.peek()) != -1) {
		if (c == '"') {//���е�'"'��Ҫע��һ��,���ܲ���ת���
			return parse_str();
		} else if (cin.peek() == 'f' || cin.peek() == 't' || cin.peek() == 'n') {
			return parse_bool();
		} else if (cin.peek() == '[') {
			return parse_arr();
		} else if (cin.peek() == '{') {
			return parse_Object();
		} else {
			return parse_num();
		}
	}
	//return val;
}
pointer parse_Object() {
	Object obj;
	cin.get();
	//obj.type = DICT;
	while (cin.peek() != '}') {
		parse_whitespace();
		//��һ��Ҫ��
		pointer p1 = parse_str();
		cout << "p1=" << p1.type << endl;
		while (cin.peek() == ' ' || cin.peek() == ':')cin.get();
		pointer p2 = parse_value();
		obj.v.push_back({p1, p2});
		while (cin.peek() != '}' && (cin.peek() == ' ' || cin.peek() == '\t' || cin.peek() == '\n' || cin.peek() == ','))cin.get();
	}
	cin.get();
	parse_whitespace();
	vec_Object.push_back(obj);
	return {OBJ, (int)vec_Object.size() - 1};
}
void dfs(pointer p) {
	if (p.type == STR) {
		cout << '"' << vec_String[p.pos].str << '"';
	} else if (p.type == NUM) {
		cout << vec_Number[p.pos].int_num;
	} else if (p.type == OBJ) {
		cout << "{";
		for (auto it = vec_Object[p.pos].v.begin(); it != vec_Object[p.pos].v.end(); it++) {
			if (it != vec_Object[p.pos].v.begin()) cout << ",";
			cout<<"\n"; 
			dfs(it->first);
			cout << ":";
			dfs(it->second);
		}
		cout << "\n}";
	} else if (p.type == BOOL) {
		cout << vec_Bool[p.pos].Bool;
	} else if (p.type == ARR) {
		cout << "[";
		for (auto x : vec_Array[p.pos].v) {
			dfs(x);
		}
		cout << "]";
	}
}
int main() {
	freopen("test.txt", "r", stdin);
	//cin.peek()�鿴��һ���ַ�������ʵ���ƶ�
	pointer point = parse_Object();
	cout << "type=" << point.type << " pos=" << point.pos << endl;
	dfs(point);
	
	return 0;
}
