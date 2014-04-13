#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;

#define NUM 5
const int thinking = 0;
const int hungry = 1;
const int eating = 2;

typedef struct Pcb {
	int num;      
	int state;
	struct Pcb *next;   //����ʱ��˳��
}Pcb, *link;
int fork[5] = { 1, 1, 1, 1, 1 };//�Ͳ棻
link ph;
void philosopher(link ph);

bool P(int &c1,int &c2) {
	if (c1 && c2) {
		c1 = c2 = 0;
		//cout << c1 << "����ѧ�һ�òͲ�" << c1 << " " << c2 << endl;
		return true;
	}
	else
		return false;
}

void V(int &c1,int &c2) {
	c1 = c2 = 1;
}
void init(link &l) {
	//for (int i = 0; i < NUM; i++) {
		l = (link)malloc(sizeof(Pcb));//�����µĽ��,����ʼ��Ϊ������
		//ph->num = i + 1;
		l->state = thinking;
		l->next = NULL;

}
void think(int i) {
	cout << i << "����ѧ������˼��������" << endl;
}
void eat() {
	link p = ph;
	while (p->next != NULL) {
		if (p->next->state == eating) {
			cout << p->next->num << "����ѧ�����ڽ��͡�����" << endl;
			V(fork[(p->next->num + 1) % 5], fork[p->next->num]);
			p->next = p->next->next;
		}
		else
			cout << p->next->num << "����ѧ���ڵȴ��Ͳ档����" << endl;
	}
	cout << "---------------------" << endl;
	philosopher(ph);

}

void getfork(int i, int t) {
	cout << t << "����ѧ�һ�òͲ�" << t << " " << (t+1) % 5 << endl;
}

void philosopher(link ph) {
	bool flag;
	link p = ph;
	if (p->next == NULL)
		return;
	while (p->next != NULL) {
		int i = p->num;
		//think(i);
		//int t = (i + 1) % NUM;
		flag = P(fork[i-1], fork[i]);
		if (flag) {
			p->state = eating;
			getfork(i-1, i);
		}
		p = p->next;
		//eat(i - 1, i);
		//V(fork[i - 1], fork[i]);
	}
	eat();
}

int main() {
	//time_t t;
	//int random;
	init(ph);
	//int i;
	//bool flag;
	int hungry[5];
	int temp,random;
	int j;
	link p,p1;
	while (1) {
		time_t t;
		srand((unsigned)time(&t));
		random = rand() % 5 + 1;//����������������
		cout << random << "λ��ѧ�Ҷ���" << endl;
		for (int h = 0; h < random; h++)
			hungry[h] = 0;
		for (int i = 0; i < random; i++) {
			srand((unsigned)time(NULL));
			temp = rand() % random + 1;
			for (j = 0; j < i; j++) {
				if (hungry[j] == temp) {
					i--;
					break;
				}
			}
			if (j == i)
				hungry[j] = temp;
		}
		for (int i = 0; i < random; i++)
			cout << hungry[i] << "\t";
		cout << endl;
		p1 = p = ph;
		for (int i2 = 0; i2 < random; i2++) {
			p->num = hungry[i2];
			p1 = p;
			init(p->next);
			p = p->next;

		}
		philosopher(ph);
		_sleep(500);
	}
	return 0;
}