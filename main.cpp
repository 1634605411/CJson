#include<bits/stdc++.h>
using namespace std;
#define STR 0
#define NUM 1
#define OBJ 2
#define BOOL 3
#define ARR 4
//所有的value类型被替换成pointer

//报错还没写 
//array没写 bool没写 

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
		//还要加个标志
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
	//之后再改
	// 0041->A
	//简单的unicode转ascii码逻辑，但是应该仅限英语
	for (int i = 0; i < 4; i++) {
		if (a[i] >= '0' && a[i] <= '9') {
			a[i] = a[i] - '0';
		} else {
			a[i] = a[i] - 'A' + 10;
		}
	}
	//记得这里写报错 不合法的返回
	//char的范围是-128~127
	return a[3] + (a[2] << 4) + (a[1] << 8) + (a[0] << 12);
}
pointer parse_str() {
	String str_tmp;
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
				str_tmp.str += c;
			} else if (c == '\\') {
				str_tmp.str += c;
			} else if (c == '/') {
				str_tmp.str += c;
			} else if (c == 'b') {
				str_tmp.str += '\b';
			} else if (c == 'f') {//formfeed 有bug 会输出一个方框
				str_tmp.str += '\f';
			} else if (c == 'n') {
				str_tmp.str += '\n';
			} else if (c == 'r') {
				str_tmp.str += '\r';
			} else if (c == 't') {
				str_tmp.str += '\t';
			} else if (c == 'u') {
				char a[4];//命名换一下
				for (int i = 0; i < 4; i++) {
					a[i] = cin.get();
				}
				str_tmp.str += UnicodeToAscii(a);
			}
			//如果是其他 还要报错
		} else if (c == '\"') {
			vec_String.push_back(str_tmp);
			return {STR, (int)vec_String.size() - 1};
		} else {
			str_tmp.str += c;
		}
	}
	//先加一个退出的地方
	vec_String.push_back(str_tmp);
	return {STR, (int)vec_String.size() - 1};
}
void parse_whitespace() {
	//还有很多种情况 比如空格 换行等 之后再看一下json文件
	//本来是if
	while (cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t') {
		cin.get();
	}
}
pointer parse_arr() {//还没写
	Array a;
	vec_Array.push_back(a);
	return {ARR, (int)vec_Array.size() - 1};
};
pointer parse_bool() {//还没写
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
		if (c == '"') {//所有的'"'都要注意一下,可能不用转义符
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
		//不一定要加
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
	//cin.peek()查看后一个字符，但是实际推动
	pointer point = parse_Object();
	cout << "type=" << point.type << " pos=" << point.pos << endl;
	dfs(point);
	
	return 0;
}
