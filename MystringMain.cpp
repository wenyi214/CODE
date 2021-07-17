#include"Mystring.h"

void test_string1(){
	my::string s1;
	my::string s2("hello");
	my::string s3(s2);

	cout << s1.c_str() << endl;
	cout << s2.c_str() << endl;
	cout << s3.c_str() << endl;

}
void test_string2(){
	my::string s1;
	my::string s2("hello");
	s1 = s2;
	cout << s1.c_str() << endl;

}

//±éÀú
void test_string3(){
	my::string s1("hello");

	for (size_t i = 0; i < s1.size(); i++){
		cout << s1[i] << ' ';
	}
	cout << endl;

	my::string::iterator it = s1.begin();
	while (it != s1.end()){
		cout << *it << ' ';
		it++;
	}
	cout << endl;

	for (auto c : s1){
		cout << c << ' ';
	}
	cout << endl;
}

void test_string4(){
	my::string s1("hello");
	my::string s2;
	//cout << s1 << endl;
	cin >> s2;
	cout << s2 << endl;

}
void test_string5(){
	my::string s1;
	s1.push_back(' ');
	s1.push_back('w');
	s1.push_back('o');

	cout << s1 << endl;
	//s1.append("rld aaaaaa");
	//cout << s1 << endl;

	//s1 += ' ';
	//s1 += 'w';
	//s1 += 'o';
	//cout << s1 << endl;
	//s1 += "rld aaaaaaaaaaaaa";
	//cout << s1 << endl;



}
void test_string6(){
	my::string s("hello");
	cout << s.size() << endl;
	cout << s.capacity() << endl;


	s.resize(10,'a');
	cout << s << endl;
	cout << s.size() << endl;
	cout << s.capacity() << endl;
	s.reserve(20);
	cout << s.size() << endl;
	cout << s.capacity() << endl;
	
}
void test_string7(){
	my::string s("hello");

	s.insert(2, 'a');
	cout << s << endl;
	cout << s.size() << endl;
	cout << s.capacity() << endl;


	s.insert(2, "xxx");
	cout << s << endl;
	cout << s.size() << endl;
	cout << s.capacity() << endl;

	s.erase(2, 3);
	cout << s << endl;
	cout << s.size() << endl;
	cout << s.capacity() << endl;

	s.erase(2);
	cout << s << endl;
	cout << s.size() << endl;
	cout << s.capacity() << endl;


}

int main(){
	//test_string1();
	//test_string2();
	//test_string3();
	test_string4();
	//test_string6();
	//test_string7();


	system("pause");
	return 0;
}