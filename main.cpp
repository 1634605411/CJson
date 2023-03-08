#include<iostream>
#include<vector>
#include<algorithm>
#include<assert.h>
using namespace std;
#define STR 0
#define INT 1
#define DOU 2
#define OBJ 3
#define BOOL 4
#define ARR 5
#define NUL 6
//1.解决string的解析问题
//2.解决number的解析问题
//3.解决whitespace的解析问题
//4.解决value的解析问题
//5.解决array的解析问题
//6.解决object的解析问题
//7.解决输出问题
//重载下标来控制输出
//所有的value类型被替换成pointer

//报错还没写
//还想写个查找功能
//再修改一下到真正的状态机
namespace Json {
	class pointer {
		public:
			int type;
			int pos;
	}root;
	class Object {
		public:
			vector<pair<pointer, pointer>>v;
	};
	class Array {
		public:
			vector<pointer>ary;//起名换一下
	};

	vector<string>vec_String;
	vector<int>vec_Number_int;
	vector<double>vec_Number_dou;
	vector<Object>vec_Object;
	vector<bool>vec_Bool;
	vector<Array>vec_Array;
	void erase() {
		vec_String.clear();
		vec_Number_int.clear();
		vec_Number_dou.clear();
		vec_Object.clear();
		vec_Bool.clear();
		vec_Array.clear();
	}

	//函数声明多写点
	pointer parse_Object();
	pointer parse_value();

	pointer parse_num() {//这里还有bug
		string s;
		//cout<<"peek="<<(char)cin.peek()<<endl;;
		while (isdigit(cin.peek()) || cin.peek() == 'e' || cin.peek() == '-' || cin.peek() == '+' || cin.peek() == '.') {
			s.push_back(cin.get());
		}
		if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'e')) {
			//cout<<"s="<<s<<endl;
			vec_Number_dou.push_back(stof(s));
			return {DOU, (int)vec_Number_dou.size() - 1};
		} else {
			//cout<<"s="<<s<<endl;
			vec_Number_int.push_back(stoi(s));
			return {INT, (int)vec_Number_int.size() - 1};
		}
	}


	char UnicodeToAscii(char hex[4]) {
		//之后再改
		// 0041->A
		//简单的unicode转ascii码逻辑，但是应该仅限英语
		for (int i = 0; i < 4; i++) {
			if (hex[i] >= '0' && hex[i] <= '9') {
				hex[i] = hex[i] - '0';
			} else {
				hex[i] = hex[i] - 'A' + 10;
			}
		}
		//记得这里写报错 不合法的返回
		//char的范围是-128~127
		return hex[3] + (hex[2] << 4) + (hex[1] << 8) + (hex[0] << 12);
	}
	pointer parse_str() {
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
				} else if (c == 'f') {//formfeed 有bug 会输出一个方框
					str_tmp += '\f';
				} else if (c == 'n') {
					str_tmp += '\n';
				} else if (c == 'r') {
					str_tmp += '\r';
				} else if (c == 't') {
					str_tmp += '\t';
				} else if (c == 'u') {
					char hex[4];//命名换一下
					for (int i = 0; i < 4; i++) {
						hex[i] = cin.get();
					}
					str_tmp += UnicodeToAscii(hex);
				}
				//如果是其他 还要报错
			} else if (c == '\"') {
				vec_String.push_back(str_tmp);
				return {STR, (int)vec_String.size() - 1};
			} else {
				str_tmp += c;
			}
		}
		//先加一个退出的地方
		vec_String.push_back(str_tmp);
		return {STR, (int)vec_String.size() - 1};
	}
	void parse_whitespace() {
		//除了string内部其他应该都能进行空白分析
		//还有很多种情况 比如空格 换行等 之后再看一下json文件
		//本来是if
		while (cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t') {
			cin.get();
		}
	}
	pointer parse_arr() {//还没写
		Array arr;
		cin.get();//吃掉  '['
		while (cin.peek() != ']') {
			parse_whitespace();
			arr.ary.push_back(parse_value());
			parse_whitespace();
			if (cin.peek() == ',') {
				cin.get();
			}
		}
		cin.get();
		vec_Array.push_back(arr);
		return {ARR, (int)vec_Array.size() - 1};

	};
	pointer parse_bool() {//还没写
		parse_whitespace();
		if (cin.peek() == 'f') {
			// false
			for (int i = 0; i < 5; i++) cin.get();
			vec_Bool.push_back(0);
		} else {
			// true
			for (int i = 0; i < 4; i++) cin.get();
			vec_Bool.push_back(1);
		}
		parse_whitespace();
		return {BOOL, (int)vec_Bool.size() - 1};
	};
	pointer parse_value() {
		parse_whitespace();
		//cout << "YES" << endl;
		while (cin.peek() != -1) {
			if (cin.peek() == '"') {//所有的'"'都要注意一下,可能不用转义符
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
		return {-1, -1};
		// 报错
	}
	pointer parse_Object() {
		Object obj;
		cin.get();
		//obj.type = DICT;
		while (cin.peek() != '}') {
			parse_whitespace();
			//不一定要加
			pointer p1 = parse_str();
			parse_whitespace();
			cin.get();
			parse_whitespace();
			pointer p2 = parse_value();
			obj.v.push_back({p1, p2});
			while (cin.peek() != '}' && (cin.peek() == ' ' || cin.peek() == '\t' || cin.peek() == '\n' || cin.peek() == ','))cin.get();
		}
		cin.get();
		parse_whitespace();
		vec_Object.push_back(obj);
		return {OBJ, (int)vec_Object.size() - 1};
	}
	void print(pointer p) {//输出部分,之后改个名字
		if (p.type == STR) {
			cout << '"' << vec_String[p.pos] << '"';
		} else if (p.type == INT) {
			cout << vec_Number_int[p.pos];
		}  else if (p.type == DOU) {
			cout << vec_Number_dou[p.pos];
		} else if (p.type == OBJ) {
			cout << "{";
			for (auto it = vec_Object[p.pos].v.begin(); it != vec_Object[p.pos].v.end(); it++) {
				if (it != vec_Object[p.pos].v.begin()) cout << ",";
				cout << "\n";
				print(it->first);
				cout << ":";
				print(it->second);
			}
			cout << "\n}";
		} else if (p.type == BOOL) {
			if (vec_Bool[p.pos]) {
				cout << "true";
			} else {
				cout << "false";
			}
		} else if (p.type == ARR) {
			cout << "[";
			for (auto it = vec_Array[p.pos].ary.begin(); it != vec_Array[p.pos].ary.end(); it++) {
				if (it != vec_Array[p.pos].ary.begin()) cout << ",";
				print(*it);
			}
			cout << "]";
		} else if (p.type == NUL) {
			cout << "null";
		} else {
			if (p.type == -1 && p.pos == -1) {
				assert(0);
			}
		}
	}
}
using namespace Json;
int main() {
	freopen("test.txt", "r", stdin);
	//cin.peek()查看后一个字符，但是实际推动
	parse_whitespace();
	//json格式可能不是一个对象，也可能是值
	while (cin.peek() != -1) {
		if (cin.peek() == '{') {//所有的'"'都要注意一下,可能不用转义符
			root = parse_Object();
		} else {
			root = parse_value();
		}
	}
	//pointer point = parse_Object();
	//cout << "type=" << point.type << " pos=" << point.pos << endl;
	print(root);

	return 0;
}
