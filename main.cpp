#include"adressbook.hpp"
#include<windows.h>


void Menu(){
	cout << "----------------------------" << endl;
	cout << "--  1.添加联系人信息      --" << endl;
	cout << "--  2.删除指定联系人信息  --" << endl;
	cout << "--  3.查找指定联系人信息  --" << endl;
	cout << "--  4.修改指定联系人信息  --" << endl;
	cout << "--  5.显示所有联系人信息  --" << endl;
	cout << "--  6.清空所有联系人      --" << endl;
	cout << "--  7.以名字排序所有联系人--" << endl;
	cout << "----8.退出                --" << endl;
	cout << "----------------------------" << endl;

}


int test(){
	AddressBook ab;
	ifstream read("addressbook.text", ifstream::app);
	read.beg;
	ab.AddressDook2File(&read);
	read.close();
	ofstream write("addressbook.text", ofstream::trunc);
	int quit = 0;
	while (!quit){
		Menu();
		int opt = 0;
		cin >> opt;
		string str;
		switch (opt){
		case 1:
			ab.AddPerson();
			break;
		case 2:
			cout << "请输入删除人的名字# ";
			
			cin >>str;
			ab.DeletePerson(str);
			break;
		case 3:
			cout << "请输入查找人的名字# ";
			
			cin >> str;
			ab.FindPerson(str);
			break;
		case 4:
			cout << "请输入修改人的名字# ";
			
			cin >> str;
			ab.ModifyMsg(str);
			break;
		case 5:
			ab.PrintBook();
			break;
		case 6:
			ab.Clear();
			break;
		case 7:
			ab.SortBook();
			break;
		case 8:
			
			ab.File2AddressDook(&write);
			write.close();
			quit = 1;
			break;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}


	system("pause");
	return 0;
}

int main(){

	test();

	/*PersonMsg ps;
	ps._name = "wen";
	ps._sex = "m";
	ps._age = "12";
	ps._telephone = "21312";
	ps._address = "ewrwer";
	File fd;
	ofstream f("addressbook.text", ofstream::trunc);
	for (int i = 0; i < 2; i++){
		fd.Write(&f, &ps);
	}
	f.close();

	ifstream r("addressbook.text", ofstream::app);
	r.beg;
	PersonMsg tmp[3];
	for (int i = 0; i < 3; i++){
		if (!fd.Read(&r,&tmp[i])){
			break;
		}
	}
*/

	
	system("pause");

	return 0;
}