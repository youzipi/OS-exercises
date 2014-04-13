#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;


#define NUM 5
typedef enum ph_state {
	thinking, eating, hungry
};	//��ѧ��״̬
typedef struct Pcb {
	ph_state state;
	int breakp;         //�ϵ㱣��
	struct Pcb *next;   //����ʱ��˳��
}Pcb, *link;

typedef struct {
	int front; //ͷָ�룬�ӷǿ�ʱָ���ͷԪ��
	int rear; //βָ�룬�ӷǿ�ʱָ���βԪ�ص���һλ��
	int number;//����������¼����Ԫ������
	
}*queue;


queue ready;
//link ready; //��������
link b_p; //empty��������
link b_c; //full��������
void p(int i);     //P����
void v(int i);     //V����
link ph[NUM];
int fork[5] = { 1, 1, 1, 1, 1 };//�Ͳ棻





void think(int i) {
	cout << i << "����ѧ������˼��������" << endl;
}
void eat(int i) {
	cout << i << "����ѧ�����ڽ��͡�����" << endl;
	fork[i] = fork[(i + 1) % NUM] = 0;
}

void p(int i) {
	cout << i << "����ѧ������Ͳ档����" << endl;

	if (fork[i] && fork[(i + 1) % NUM]) {
		ph[i]->state = eating;
		cout << "����ɹ�������" << endl;
	}
	else {
		ph[i]->state = hungry;
		ready->rear = i;
		rear++;
		cout << "����ʧ�ܣ�����ȴ����С�����" << endl;
	}
}

void v(int i) {
	fork[i] = fork[(i + 1) % NUM] = 1;
	cout << i << "����ѧ���ͷŲͲ档����" << endl;
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

		rd = (rand() % 5);;//����������������
		cout << "random:" << rd << endl;
		_sleep(1000);
		philosopher(rd);
	}

	return 0;
}