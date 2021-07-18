#include"MystringNew.h"
void test_String1(){
	String s1;
	for (size_t i = 0; i < s1.size(); i++){
		cout << s1[i] << " ";
	}
	cout << endl;
	String s2("hello");
	String s3(s2);

	s1 = s2;

	for (size_t i = 0; i < s1.size(); i++){
		cout << s1[i] << " ";
	}
	cout << endl;

	for (size_t i = 0; i < s2.size(); i++){
		cout << s2[i] << " ";
	}
	cout << endl;

	for (size_t i = 0; i < s3.size(); i++){
		cout << s3[i] << " ";
	}
	cout << endl;
}


int main(){
	test_String1();
	system("pause");
	return 0;
}