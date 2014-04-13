#include "a.h"



//���ݽṹ�Ķ����ȫ�ֱ���
typedef struct Pcb {
	char name[10];      //������
	char state[10];    //����״̬
	char reason[10];    //����������ԭ��
	int breakp;         //�ϵ㱣��
	struct Pcb *next;   //����ʱ��˳��
}Pcb, *link;
int empty, full; //�ź���
link p1;//�����߽���
link c1;//�����߽���

int buffer[MAX]; //�����
int len; //���볤��
int space[MAX];
int sp = 0; //string��ָ��
int in = 0; //������ָ��
int out = 0; //������ָ��
int temp; //����ӡ����ʱ��Ʒ
int rec_p[100];//������¼
int rp1 = 0;//������¼ָ��
int rec_c[100];//���Ѽ�¼
int rp2 = 0;//���Ѽ�¼ָ��
link ready; //��������
link b_p; //empty��������
link b_c; //full��������
int flag; //���������

int con_cnt; //���Ѽ�����

void init();       //��ʼ��
void p(int s);     //P����
void v(int s);     //V����
void block(int s);//��������
void wakeup(int s);//���Ѻ���
void control();   //���������
void processor();//�����ִ��
void print();  //��ӡ����


void init() { //��ʼ��
	empty = MAX;
	full = 0;
	for (int i = 0; i < MAX; i++) {
		space[i] = i + 1;
	}
	p1 = (link)malloc(sizeof(Pcb));//�����µĽ��,����ʼ��Ϊ������
	strcpy(p1->name, "Producer");
	strcpy(p1->state, "Ready");
	strcpy(p1->reason, "Null");
	p1->breakp = 0;
	p1->next = NULL;

	c1 = (link)malloc(sizeof(Pcb));//�����µĽ��,����ʼ��Ϊ������
	strcpy(c1->name, "Consumer");
	strcpy(c1->state, "Ready");
	strcpy(c1->reason, "Null");
	c1->breakp = 0;
	c1->next = NULL;

	ready = p1;
	ready->next = c1;//��ʼ��Ϊ����������ǰ�����ѽ����ں�
	c1->next = NULL;
	b_p = NULL;
	b_c = NULL;//��������ΪNULL
	flag = 0;
	con_cnt = 0; //���Ѽ�����
}

void p(int s) {
	if (s == 1) { //p(empty)
		empty--;
		if (empty<0)
			block(1); //������ǰ��������
		else {
			printf("\t* �������пռ�!\n\t* ����ռ�-1\n");
			ready->breakp = flag; //����ϵ�
		}
	}
	else { //p(full)
		full--;
		if (full<0)
			block(2);//������ǰ���ѽ���
		else {
			printf("\t* ���������ַ�!\n\t* ����ռ�+1\n");
			ready->breakp = flag; //����ϵ�
		}
	}
}
void v(int s) {
	if (s == 1) { //v(empty)
		empty++;
		if (empty <= 0)
			wakeup(1); //������������
		ready->breakp = flag; //����ϵ�
	}
	else { //v(full)
		full++;
		if (full <= 0)
			wakeup(2);//�������ѽ���
		ready->breakp = flag; //����ϵ�
	}
}

void block(int s) {//���������Ķ���
	link p;
	int num1 = 0;
	int num2 = 0;
	if (s == 1) {//��������
		strcpy(p1->state, "Block");//�ı�״̬
		strcpy(p1->reason, "FULL");//˵��ԭ��
		p = b_p;
		while (p) {
			num1++;
			p = p->next;//p��ֵΪNULL,��ʾ��β
		}
		//if (!b_p)//������������Ϊ�գ���p1����
		//	b_p = p1;
		//else
			p = p1;
		p1->next = NULL;
		printf("\t* p1��������������!\n");
		ready->breakp = flag; //����ϵ�
		ready = ready->next;//�ھ���������ȥ��,ָ����һ��	
		num1++;
	}
	else {//���ѽ���
		strcpy(c1->state, "Block");
		strcpy(c1->reason, "EMPTY");
		p = b_c;
		while (p) {
			num2++;
			p = p->next;//p��ֵΪNULL,��ʾ��β
		}
		if (!b_c)
			b_c = c1;
		else
			p = c1;
		ready->breakp = flag; //����ϵ�
		ready = ready->next;//�ھ���������ȥ��,ָ����һ��		
		c1->next = NULL;
		printf("\t* c1���ѽ���������!\n");
		num2++;
	}
	//printf("\t* �������������̸���Ϊ:%d\n", num1);
	//printf("\t* ���������ѽ��̸���Ϊ:%d\n", num2);
}

void wakeup(int s) {//���Ѻ����Ķ���
	link p;
	link q = ready;
	if (s == 1) { //����b_p���׽���,�������̶���
		p = b_p;
		b_p = b_p->next;//����ָ��ָ����һ����������
		strcpy(p->state, "Ready");
		strcpy(p->reason, "Null");
		while (q)//�����������
			q = q->next;
		q = p;
		p->next = NULL;
		printf("\t* p1�������̻�����!\n");
	}
	else { //����b_c���׽���,���ѽ��̶���
		p = b_c;
		b_c = b_c->next;//����ָ��ָ����һ����������
		strcpy(p->state, "Ready");
		strcpy(p->reason, "Null");
		while (q->next)//�����������
			q = q->next;
		q->next = p;
		p->next = NULL;
		printf("\t* c1���ѽ��̻�����!\n");
	}
}

void control() //���������ȳ���
{
	int rd;
	int num = 0;

	link p = ready;
	while (p) //ͳ�ƾ������̸�������ready���������
	{
		num++;
		p = p->next;
	}
	printf("\t* �������̸���Ϊ:%d\n", num);

	time_t t;
	srand((unsigned)time(&t));
	rd = rand() % num;//����������������


	cout <<"random:"<< rd << endl;

	if (rd == 1) {	//readyǰ�����
		cout << "������������ǰ����" << endl;
		p = ready;
		ready = ready->next;
		ready->next = p;
		p->next = NULL;
		strcpy(ready->state, "Run");
		strcpy(ready->next->state, "Ready");
	}
	//�����������о������еĵ�һ������
	else
		strcpy(ready->state, "Run");
	flag = ready->breakp;	//����ϵ�
}

void processor() { //ģ�⴦����ָ��ִ��
	if (strcmp(ready->name, "Producer") == 0) //��ǰ����Ϊ������
		switch (flag) {
			case 0://produce
				printf("\t* �������������ַ�%d\n", space[sp]);	
				rec_p[rp1] = space[sp];
				rp1++;
				//rp1 = (rp1 + 1) % MAX;			
				flag++;
				ready->breakp = flag; //����ϵ�
				break;
			case 1: //p(empty)
				flag++;
				p(1);
				break;
			case 2: //put
				buffer[in] = rec_p[sp]; //�ŵ�������
				printf("\t* %d�ɹ����뻺����!\n", buffer[in]);	
				sp = (sp + 1) % MAX;
				in = (in + 1) % MAX;
				flag++;
				ready->breakp = flag; //����ϵ�
				break;
			case 3: //v(full)
				//flag++;
				printf("\t* �ַ����뻺�����������ռ�-1\n");
				printf("\t* �������̶ϵ��� 0 \n");
				flag = 0;
				v(2);
				break;
			//case 4://goto00  

			//	ready->breakp = flag; //����ϵ�
			//	break;

	}
	else  //��ǰ����Ϊ������
		switch (flag) {
			case 0: //p(full)
				flag++;
				p(2);
				break;
			case 1: //get
				printf("\t* ������ȡ�ַ�!\n");
				temp = buffer[out];
				out = (out + 1) % MAX;
				flag++;
				ready->breakp = flag; //����ϵ�
				break;
			case 2: //v(empty)
				flag++;
				printf("\t* �ַ��뿪�������������ռ�+1\n");

				v(1);
				break;
			case 3: //consume
				printf("\t* �������ַ�%d\n", temp);
				rec_c[rp2] = temp;//��ӵ����Ѽ�¼
				//rp2 = (rp2 + 1) % MAX;
				rp2++;
				//flag++;
				printf("\t* ���ѽ��̶ϵ��� 0 \n");
				flag = 0;
				ready->breakp = flag; //����ϵ�
				break;
			//case 4: //goto0
				//ready->breakp = flag; //����ϵ�
				//break;
	}
}

void print() {
	int i, j;

	printf("\n* ������:");
	for (j = 0; j <= rp1; j++)
		if (buffer[j] != 0)
			printf("%d ", rec_p[j]);
	printf("\nrec_p[%d]:%d", rp1, rec_p[rp1]);
	cout << endl<<rp1 << endl;
	printf("\n* ������:");
	for (j = out; j <= in; j++)
		if (buffer[j] != 0)
			printf("%d ", buffer[j]);
		cout << endl << in - out << endl;
	printf("\n* ������:");
	for (j = 0; j <= rp2; j++)
		if (buffer[j] != 0)
			printf("%d ", rec_c[j]);
		cout << endl << rp2 << endl;
	printf("\n�����������������̿��ƿ����Ϣ����������������\n");
	printf("������\t\t״̬\t�ȴ�ԭ��\t�ϵ�\n");
	printf("%s\t%s\t%s\t\t%d\n\n", p1->name, p1->state, p1->reason, p1->breakp);
	printf("%s\t%s\t%s\t\t%d\n", c1->name, c1->state, c1->reason, c1->breakp);
	printf("����������������������������������������������\n");
	printf("1.���� 0.�˳�\n");
	//getchar();

}




void main() {
	printf("*������������ģ��\n");
	printf("������������������\n");

	init();        //��ʼ��
	while(1){
		printf("������������������ģ��ָ�����̡���������������\n");
		control();      //���������ȳ���
		processor();    //ģ�⴦����ָ��ִ��
		print();        //�����ʾ������Ϣ
	}
}
