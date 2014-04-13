#include<iostream>
#include<time.h>

using namespace std;


int main() {

	time_t t;
	int random;
	while (1) {
		srand((unsigned)time(&t));
		random = rand() % 2;//随机函数产生随机数
		cout << "random:" << random << endl;
		_sleep(1500);
	}
	return 0;
}