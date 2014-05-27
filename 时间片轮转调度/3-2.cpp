#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "time.h"
using namespace std;

const int N = 5; /*进程数*/

typedef struct PCB {
	//char name[10];  /*进程标识符*/
	int name;
	//int state;
	char state;
	int priority;   /*进程优先数*/
	int runtime;	//已运行时间
	int time; /*进程到完成还要的时间*/
	struct PCB *next; /*链指针*/
}PCB, *link;
PCB *finish = 0, *ready;// *run; /*队列指针*/
PCB pcbs[N];
PCB *p;




void init() {
	int i;
	for (i = 0; i < N; i++) {
		time_t t;
		srand((unsigned)time(&t));
		int random = rand() % 5 + 1;//随机函数产生随机数 
		pcbs[i].name = i;
		pcbs[i].time = random * 2 + 1;
		//cout << pcbs[i].time;

		pcbs[i].runtime = 0;
		pcbs[i].state = 'R';

		_sleep(1100);
	}
}

PCB * initList(PCB pcb[]) {
	PCB *pHead;
	PCB *pTail;
	PCB *pnode;
	int i;
	pHead = (PCB *)malloc(sizeof(PCB));
	pTail = (PCB *)malloc(sizeof(PCB));
	pHead = pTail;
	for (i = 0; i < N; i++) {
		pnode = (PCB *)malloc(sizeof(PCB));
		*pnode = pcb[i];
		pTail->next = pnode;
		pTail = pTail->next;
	}
	pTail->next = pHead->next;
	return pHead;
	//return pHead->next;
}

PCB * insert(PCB *p, PCB *pnode) {
	PCB *ph = p;
	pnode->next = ph;
	//ph->next = pnode;
	return pnode;
}

void print() {
	PCB *p1 = finish;
	PCB *p2 = ready;
	int finish_num = 0;
	cout << "name" << "	time" << "	runtime" << "	state" << endl;
	while (p1 != NULL) {
		cout << p1->name << "	" << p1->time << "	" << p1->runtime <<"	"<< p1->state <<endl;
		p1 = p1->next;
		finish_num++;
	}
	//while (p2 != NULL) {
	int ready_num = N - finish_num;
	while (ready_num--) {
		cout << p2->next->name << "	" << p2->next->time << "	" << p2->next->runtime <<"	"<< p2->next->state <<endl;
		p2 = p2->next;
	}


}

void run(PCB *p) {
	//int i, j;
	PCB *ph = p;
	PCB *t;
	while (ph != ph->next) {
		ph->next->runtime++;
		cout << "name" << "	time" << "	runtime" << endl;
		cout << ph->next->name << "	" << ph->next->time << "	" << p->next->runtime << endl;
		if (ph->next->time == ph->next->runtime) {
			t = ph->next;
			t->state = 'E';
			ph->next = ph->next->next;
			t->next = 0;
			finish = insert(finish, t);
			ready = p;
			print();
		}
		ph = ph->next;
	}

}


int main() {
	init();

	p = initList(pcbs);
	run(p);
	return 0;
}