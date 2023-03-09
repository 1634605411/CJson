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
//1.���string�Ľ�������
//2.���number�Ľ�������
//3.���whitespace�Ľ�������
//4.���value�Ľ�������
//5.���array�Ľ�������
//6.���object�Ľ�������
//7.����������
//�����±����������
//���е�value���ͱ��滻��pointer

//����ûд
//����д�����ҹ���
//���޸�һ�µ�������״̬��


namespace Json {
	string json_str;
	int now_pos = 0;
	char Jsonget() {
		return json_str[now_pos++];
	}
	char Jsonpeek() {
		if (now_pos == (int)json_str.size())return -1;
		return json_str[now_pos];
	}
	class pointer {
		public:
			int type;
			int pos;
	} root;
	//root node
	class Object {
		public:
			vector<pair<pointer, pointer>>v;
	};
	class Array {
		public:
			vector<pointer>ary;//������һ��
	};

	vector<string>vec_String;
	vector<int>vec_Number_int;
	vector<double>vec_Number_dou;
	vector<Object>vec_Object;
	vector<bool>vec_Bool;
	vector<Array>vec_Array;

	void erase() {
		json_str.erase();
		now_pos = 0;
		vec_String.clear();
		vec_Number_int.clear();
		vec_Number_dou.clear();
		vec_Object.clear();
		vec_Bool.clear();
		vec_Array.clear();
	}

	//����������д��
	pointer parse_Object();
	pointer parse_value();

	pointer parse_num() {//�����ϸ�LL1�ĸо�
		string s;
		//cout<<"peek="<<(char)Jsonpeek()<<endl;;
		while (isdigit(Jsonpeek()) || Jsonpeek() == 'e' || Jsonpeek() == '-' || Jsonpeek() == '+' || Jsonpeek() == '.') {
			s.push_back(Jsonget());
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
		//֮���ٸ�
		// 0041->A
		//�򵥵�unicodeתascii���߼�������Ӧ�ý���Ӣ��
		for (int i = 0; i < 4; i++) {
			if (hex[i] >= '0' && hex[i] <= '9') {
				hex[i] = hex[i] - '0';
			} else {
				hex[i] = hex[i] - 'A' + 10;
			}
		}
		//�ǵ�����д���� ���Ϸ��ķ���
		//char�ķ�Χ��-128~127
		return hex[3] + (hex[2] << 4) + (hex[1] << 8) + (hex[0] << 12);
	}
	pointer parse_str() {
		string str_tmp;
		Jsonget();
		char c;
		//���ܿ�����ܴ�֮��һ���ܲ����Ż�
		//	ԭ���� while ((Jsonpeek() != -1) && (c = Jsonget()) != '\"') {
		while ( Jsonpeek() != -1) {
			c = Jsonget();
			//if�������޳�switch���
			//��ͬ���ܵĿ��Ժϲ�
			if (c == '\\') {
				c = Jsonget();
				if (c == '\"') {
					str_tmp += c;
				} else if (c == '\\') {
					str_tmp += c;
				} else if (c == '/') {
					str_tmp += c;
				} else if (c == 'b') {
					str_tmp += '\b';
				} else if (c == 'f') {//formfeed ��bug �����һ������
					str_tmp += '\f';
				} else if (c == 'n') {
					str_tmp += '\n';
				} else if (c == 'r') {
					str_tmp += '\r';
				} else if (c == 't') {
					str_tmp += '\t';
				} else if (c == 'u') {
					char hex[4];//������һ��
					for (int i = 0; i < 4; i++) {
						hex[i] = Jsonget();
					}
					str_tmp += UnicodeToAscii(hex);
				}
				//��������� ��Ҫ����
			} else if (c == '\"') {
				vec_String.push_back(str_tmp);
				return {STR, (int)vec_String.size() - 1};
			} else {
				str_tmp += c;
				//cout<<"str="<<str_tmp<<endl;
				//cout<<"c="<<(int)c<<endl;
			}
		}
		//�ȼ�һ���˳��ĵط�
		vec_String.push_back(str_tmp);
		return {STR, (int)vec_String.size() - 1};
	}
	void parse_whitespace() {
		//����string�ڲ�����Ӧ�ö��ܽ��пհ׷���
		//���кܶ������ ����ո� ���е� ֮���ٿ�һ��json�ļ�
		//������if
		while (Jsonpeek() == ' ' || Jsonpeek() == '\n' || Jsonpeek() == '\t') {
			Jsonget();
		}
	}
	pointer parse_arr() {//��ûд
		Array arr;
		Jsonget();//�Ե�  '['
		while (Jsonpeek() != ']') {
			parse_whitespace();
			arr.ary.push_back(parse_value());
			parse_whitespace();
			if (Jsonpeek() == ',') {
				Jsonget();
			}
		}
		Jsonget();
		vec_Array.push_back(arr);
		return {ARR, (int)vec_Array.size() - 1};

	};
	pointer parse_bool() {//��ûд
		parse_whitespace();
		if (Jsonpeek() == 'f') {
			// false
			for (int i = 0; i < 5; i++) Jsonget();
			vec_Bool.push_back(0);
		} else {
			// true
			for (int i = 0; i < 4; i++) Jsonget();
			vec_Bool.push_back(1);
		}
		parse_whitespace();
		return {BOOL, (int)vec_Bool.size() - 1};
	};
	pointer parse_value() {
		parse_whitespace();
		//cout << "YES" << endl;
		while (Jsonpeek() != -1) {
			if (Jsonpeek() == '"') {//���е�'"'��Ҫע��һ��,���ܲ���ת���
				return parse_str();
			} else if (Jsonpeek() == 'f' || Jsonpeek() == 't' || Jsonpeek() == 'n') {
				return parse_bool();
			} else if (Jsonpeek() == '[') {
				return parse_arr();
			} else if (Jsonpeek() == '{') {
				return parse_Object();
			} else {
				return parse_num();
			}
		}
		return {-1, -1};
		// ����
	}
	pointer parse_Object() {
		Object obj;
		Jsonget();
		//obj.type = DICT;
		while (Jsonpeek() != '}') {
			parse_whitespace();
			//��һ��Ҫ��
			pointer p1 = parse_str();
			parse_whitespace();
			Jsonget();
			parse_whitespace();
			pointer p2 = parse_value();
			obj.v.push_back({p1, p2});
			while (Jsonpeek() != '}' && (Jsonpeek() == ' ' || Jsonpeek() == '\t' || Jsonpeek() == '\n' || Jsonpeek() == ','))Jsonget();
		}
		Jsonget();
		parse_whitespace();
		vec_Object.push_back(obj);
		return {OBJ, (int)vec_Object.size() - 1};
	}
	void print(pointer p) {//�������,֮��ĸ�����
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
	//��ȡ���� ����д��һ������
	//ע��test�ĳ�ANSI����,�޷���������UTF-8��ʽ������ 
	freopen("test.txt", "r", stdin);
	string tmp;
	while (getline(cin, tmp)) {
		json_str += tmp;
		if (Jsonpeek() != -1) //Jsonpeek()����Ƿ�����ļ�β
			json_str += '\n';
	}

	parse_whitespace();
	//json��ʽ���ܲ���һ������Ҳ������ֵ
	while (Jsonpeek() != -1) {
		if (Jsonpeek() == '{') {//���е�'"'��Ҫע��һ��,���ܲ���ת���
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
