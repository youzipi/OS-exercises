#pragma execution_character_set("utf-8")
#include<iostream>
#include<time.h>
#include<stdlib.h>
using namespace std;

#define MAX 20
#define N 2
//const int block = 0;
//const int ready = 1;

const int Producer = 0;
const int Consumer = 1;

int buf[MAX];
int mutex = 1;
int empty = MAX;
int full = 0;
int in = 0;	//生产指针
int out = 0;//消费指针

typedef struct Pcb {
	int name;      //进程名,p ,c
	int num;         
}Pcb, *link;

link p[N];
link c[N];
void init() {
	for (int i = 0; i < N; i++) {
		p[i] = (link)malloc(sizeof(Pcb));//建立新的结点,并初始化为生产者
		p[i]->name = Producer;
		p[i]->num = i+1;
		//p[i]->state = ready;
	}
	for (int i = 0; i < N; i++) {
		c[i] = (link)malloc(sizeof(Pcb));//建立新的结点,并初始化为生产者
		c[i]->num = i + 1;
		c[i]->name = Consumer;
		//c[i]->state = ready;
	}
}
bool P(int &item) {
	if (item) {
		item--;
		return true;
	}
	else	
		return false;
}

void V(int &item) {
	item++;
}

bool producer(link p) {
	bool key;
	key = P(empty);
	if (!key) {
		//p->state = block;
		return false;
	}
	key = P(mutex);
	if (!key) {
		//p->state = block;
		return false;
	}
	//cout << "produce..." << endl;
	buf[in] = in+1;
	in = (in + 1) % MAX;
	V(mutex);
	V(full);
	return true;
}

bool consumer(link p) {
	bool key;
	key = P(full);
	if (!key) {
		//p->state = block;
		return false;
	}
	key = P(mutex);
	if (!key) {
		//p->state = block;
		return false;
	}
	//cout << "consume..." << endl;
	if (in == out)
		return false;
	buf[out] = 0;
	out = (out + 1) % MAX;
	V(mutex);
	V(empty);
	return true;
}

bool process(link t) {

	if (t->name == Producer) {
		return producer(t);
	}
	else
		return consumer(t);
}
void print(link t) {
	if (t->name == Producer)
		cout << "Producer:" << t->num <<"\tin: " << (in == 0 ? MAX : in )<< endl;
	else
		cout << "Consumer:" << t->num << "\tout:" << (out == 0 ? MAX : out) << endl;
	for (int i = 0; i < MAX; i++)
		cout << buf[i] << " ";
	cout << endl;
}
int main() {

	time_t t;
	int random;
	init();
	int i;
	bool flag;
	link temp;
	while (1) {

		srand((unsigned)time(&t));
		random = rand() % 4;//随机函数产生随机数
		i = random % 2;
		random = random / 2;
		cout << "random:" << random << endl;
		if (full == 0) {	//缓存区为空，执行生产者进程
			temp = p[i];
		}
		else if (empty == 0) {//缓存区已满，执行消费者进程
			temp = c[i];
		}
		else if (random) {	//根据随机数执行进程
			temp = p[i];
		}
		else {
			temp = c[i];
		}

		flag = process(temp);
		if (flag)
			print(temp);
		_sleep(1500);
	}
	return 0;
}