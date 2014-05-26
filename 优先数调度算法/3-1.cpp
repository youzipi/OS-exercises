#include "iostream"
#include "stdlib.h"
#include "string.h"
using namespace std;

const int N = 5; /*进程数*/

typedef struct PCB {
	//char name[10];  /*进程标识符*/
	int name;
	int state;
	int priority;   /*进程优先数*/
	int time; /*进程到完成还要的时间*/
	struct PCB *next; /*链指针*/
}PCB,*link;
//PCB *finish, *ready;// *run; /*队列指针*/
PCB pcbs[N];
PCB *p;


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
	pTail->next = 0;
	return pHead;
}

void init() {
	int i;
	for (i = 0; i < N; i++) {
		pcbs[i].name = i;
		pcbs[i].time = i+2;
		pcbs[i].priority = 2*i;
	}
}

PCB * insert(PCB *p, PCB *pnode) {
	PCB *ph = p;
	while (ph->next != NULL) {
		if (pnode->priority >= ph->next->priority) {
			pnode->next = ph->next;
			//ph->next = 0;
			ph->next = pnode;
			return p;
		}
		else
			ph = ph->next;
	}
	return p;
}
void print(PCB *p) {
	link ph = p;
	cout << "name priority " << endl;
	while (NULL != ph->next) {
		cout << ph->next->name << " " << ph->next->priority << endl;
		ph = ph->next;
	}
}

void sort() {
	p = (PCB * )malloc(sizeof(PCB));

	link pHead = p;
	link pTmp;
	int i,j;
	//int index = i;
	PCB temp;
	//int max_priority = pcbs[i].priority;
	for (i = 0; i < N - 1; i++) {
		for (j = i+1; j < N; j++) {
			if (pcbs[j].priority > pcbs[i].priority) {
				temp = pcbs[i];
				pcbs[i] = pcbs[j];
				pcbs[j] = temp;
			}
		}
	}
	p = initList(pcbs);
	print(p);

}



void privilege(PCB *p) {
	int i, j;
	PCB *ph = p;
	PCB *t;
	while (NULL != ph->next){
		ph->next->priority--;
		ph->next->time--;
		cout << "name" <<"	time" <<"	priority" << endl;
		cout << ph->next->name <<"	"<< ph->next->priority <<"	"<< p->next->time << endl;
		t = ph->next;
		ph->next = ph->next->next;
		if ((0 != ph->next) && (t->time != 0)) {
			p = insert(p, t);
		}
		else
			t->state = 1;
		print(p);
		ph = p;
	}
}


int main() {
	//PCB *p = NULL;
	//initList();
	init();
	sort();
	privilege(p);
	return 0;
}