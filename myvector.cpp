#include"vector.h"
void test_vector1(){
	vector<int> v1;
	cout << v1.size() << endl;
	cout << v1.capacity() << endl;

	v1.reserve(10);
	cout << v1.size() << endl;
	cout << v1.capacity() << endl;

}

void test_vector2(){
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	//v1.insert(v1.begin() + 2, 5);
	for (size_t i = 0; i < v1.size(); i++){
		cout << v1[i] << " ";
	}
	cout << endl;

	//v1.pop_back();
	//v1.pop_back();
	v1.erase(v1.begin() + 2);
	for (size_t i = 0; i < v1.size(); i++){
		cout << v1[i] << " ";
	}
	cout << endl;



}

void test_vector3(){
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	v1.push_back(5);

	for (size_t i = 0; i < v1.size(); i++){
		v1[i] += 1;
		cout << v1[i] << " ";
	}
	cout << endl;

	vector<int>::iterator it = v1.begin();
	while (it != v1.end()){
		*it -= 1;
		cout << *it << " ";
		it++;
	}
	cout << endl;

	for (auto c : v1){
		cout << c << " ";
	}
	cout << endl;

	vector<int>::const_iterator cit = v1.cbegin();
	while (cit != v1.end()){
		//²»¿ÉÐÞ¸Ä
		//*cit = 1;
		cout << *cit << " ";
		cit++;
	}

}

void test_vector4(){
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	v1.resize(2);
	for (auto c : v1){
		cout << c << " ";
	}
	cout << endl;
	cout << v1.size() << endl;
	cout << v1.capacity() << endl << endl;

	v1.resize(5, 5);
	for (auto c : v1){
		cout << c << " ";
	}
	cout << endl;
	cout << v1.size() << endl;
	cout << v1.capacity() << endl << endl;
}

void test_vector5(){
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	vector<int> v2(v1);
	vector<int> v3;

	v3 = v1;

	for (auto c : v2){
		cout << c << " ";
	}
	cout << endl;
	cout << v2.size() << endl;
	cout << v2.capacity() << endl << endl;
	for (auto c : v3){
		cout << c << " ";
	}
	cout << endl;
	cout << v3.size() << endl;
	cout << v3.capacity() << endl << endl;
}

void test_vector6(){
	vector<int> v1(3, 1);
	for (auto c : v1){
		cout << c << " ";
	}
	cout << endl;
	cout << v1.size() << endl;
	cout << v1.capacity() << endl << endl;
}


int main(){
	test_vector6();
	system("pause");
	return 0;
}