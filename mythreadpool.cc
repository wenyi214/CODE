#include"mythreadpool.hpp"
#include  <unistd.h>
int main(){
	mythreadpool *tp = new mythreadpool();
	tp->threadinit();
	int count = 10;
	while (1){
		//sleep(1);
		int x = rand() % 10 + 1;
		int y = rand() % 20 + 1;

		Task t(x, y);
		//put里有加锁，不需要加锁了
		tp->Put(t);
		std::cout << "put Task done..." << std::endl;
		count--;
		if (count == 0){
			break;
		}
	}

	//sleep(3);
	tp->ThreadQuit();
	delete tp;


	return 0;
}
