#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;


#define NUM 5
typedef enum ph_state {
	thinking, eating, hungry
};	//哲学家状态
typedef struct Pcb {
	ph_state state;
	int breakp;         //断点保护
	struct Pcb *next;   //阻塞时的顺序
}Pcb, *link;

typedef struct {
	int front; //头指针，队非空时指向队头元素
	int rear; //尾指针，队非空时指向队尾元素的下一位置
	int number;//计数器，记录队中元素总数
	
}*queue;


queue ready;
//link ready; //就绪队列
link b_p; //empty阻塞队列
link b_c; //full阻塞队列
void p(int i);     //P操作
void v(int i);     //V操作
link ph[NUM];
int fork[5] = { 1, 1, 1, 1, 1 };//餐叉；





void think(int i) {
	cout << i << "号哲学家正在思考。。。" << endl;
}
void eat(int i) {
	cout << i << "号哲学家正在进餐。。。" << endl;
	fork[i] = fork[(i + 1) % NUM] = 0;
}

void p(int i) {
	cout << i << "号哲学家请求餐叉。。。" << endl;

	if (fork[i] && fork[(i + 1) % NUM]) {
		ph[i]->state = eating;
		cout << "请求成功。。。" << endl;
	}
	else {
		ph[i]->state = hungry;
		ready->rear = i;
		rear++;
		cout << "请求失败，进入等待队列。。。" << endl;
	}
}

void v(int i) {
	fork[i] = fork[(i + 1) % NUM] = 1;
	cout << i << "号哲学家释放餐叉。。。" << endl;
}





void philosopher(int i) {
	int flag;
	while (true) {
		think(i);
		int t = (i + 1) % NUM;
		p(i);
		if (ph[i]->state == eating)
			eat(i);
		//v(fork[i], fork[(i + 1) % NUM]);
		v(i);
	}
}

int main() {
	time_t t;
	int rd;
	while (1) {
		srand((unsigned)time(NULL));

		rd = (rand() % 5);;//随机函数产生随机数
		cout << "random:" << rd << endl;
		_sleep(1000);
		philosopher(rd);
	}

	return 0;
}