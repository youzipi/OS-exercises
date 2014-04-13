#include "a.h"



//数据结构的定义和全局变量
typedef struct Pcb {
	char name[10];      //进程名
	char state[10];    //运行状态
	char reason[10];    //若阻塞，其原因
	int breakp;         //断点保护
	struct Pcb *next;   //阻塞时的顺序
}Pcb, *link;
int empty, full; //信号量
link p1;//生产者进程
link c1;//消费者进程

int buffer[MAX]; //缓冲池
int len; //输入长度
int space[MAX];
int sp = 0; //string的指针
int in = 0; //生产者指针
int out = 0; //消费者指针
int temp; //供打印的临时产品
int rec_p[100];//生产记录
int rp1 = 0;//生产记录指针
int rec_c[100];//消费记录
int rp2 = 0;//消费记录指针
link ready; //就绪队列
link b_p; //empty阻塞队列
link b_c; //full阻塞队列
int flag; //程序计数器

int con_cnt; //消费计数器

void init();       //初始化
void p(int s);     //P操作
void v(int s);     //V操作
void block(int s);//阻塞函数
void wakeup(int s);//唤醒函数
void control();   //处理机调度
void processor();//处理机执行
void print();  //打印函数


void init() { //初始化
	empty = MAX;
	full = 0;
	for (int i = 0; i < MAX; i++) {
		space[i] = i + 1;
	}
	p1 = (link)malloc(sizeof(Pcb));//建立新的结点,并初始化为生产者
	strcpy(p1->name, "Producer");
	strcpy(p1->state, "Ready");
	strcpy(p1->reason, "Null");
	p1->breakp = 0;
	p1->next = NULL;

	c1 = (link)malloc(sizeof(Pcb));//建立新的结点,并初始化为消费者
	strcpy(c1->name, "Consumer");
	strcpy(c1->state, "Ready");
	strcpy(c1->reason, "Null");
	c1->breakp = 0;
	c1->next = NULL;

	ready = p1;
	ready->next = c1;//初始化为生产进程在前，消费进程在后
	c1->next = NULL;
	b_p = NULL;
	b_c = NULL;//阻塞进程为NULL
	flag = 0;
	con_cnt = 0; //消费计数器
}

void p(int s) {
	if (s == 1) { //p(empty)
		empty--;
		if (empty<0)
			block(1); //阻塞当前生产进程
		else {
			printf("\t* 缓存区有空间!\n\t* 缓存空间-1\n");
			ready->breakp = flag; //保存断点
		}
	}
	else { //p(full)
		full--;
		if (full<0)
			block(2);//阻塞当前消费进程
		else {
			printf("\t* 缓存区有字符!\n\t* 缓存空间+1\n");
			ready->breakp = flag; //保存断点
		}
	}
}
void v(int s) {
	if (s == 1) { //v(empty)
		empty++;
		if (empty <= 0)
			wakeup(1); //唤醒生产进程
		ready->breakp = flag; //保存断点
	}
	else { //v(full)
		full++;
		if (full <= 0)
			wakeup(2);//唤醒消费进程
		ready->breakp = flag; //保存断点
	}
}

void block(int s) {//阻塞函数的定义
	link p;
	int num1 = 0;
	int num2 = 0;
	if (s == 1) {//阻塞生产
		strcpy(p1->state, "Block");//改变状态
		strcpy(p1->reason, "FULL");//说明原因
		p = b_p;
		while (p) {
			num1++;
			p = p->next;//p的值为NULL,表示队尾
		}
		//if (!b_p)//生产阻塞队列为空，则将p1放入
		//	b_p = p1;
		//else
			p = p1;
		p1->next = NULL;
		printf("\t* p1生产进程阻塞了!\n");
		ready->breakp = flag; //保存断点
		ready = ready->next;//在就绪队列中去掉,指向下一个	
		num1++;
	}
	else {//消费进程
		strcpy(c1->state, "Block");
		strcpy(c1->reason, "EMPTY");
		p = b_c;
		while (p) {
			num2++;
			p = p->next;//p的值为NULL,表示队尾
		}
		if (!b_c)
			b_c = c1;
		else
			p = c1;
		ready->breakp = flag; //保存断点
		ready = ready->next;//在就绪队列中去掉,指向下一个		
		c1->next = NULL;
		printf("\t* c1消费进程阻塞了!\n");
		num2++;
	}
	//printf("\t* 阻塞的生产进程个数为:%d\n", num1);
	//printf("\t* 阻塞的消费进程个数为:%d\n", num2);
}

void wakeup(int s) {//唤醒函数的定义
	link p;
	link q = ready;
	if (s == 1) { //唤醒b_p队首进程,生产进程队列
		p = b_p;
		b_p = b_p->next;//阻塞指针指向下一个阻塞进程
		strcpy(p->state, "Ready");
		strcpy(p->reason, "Null");
		while (q)//插入就绪队列
			q = q->next;
		q = p;
		p->next = NULL;
		printf("\t* p1生产进程唤醒了!\n");
	}
	else { //唤醒b_c队首进程,消费进程队列
		p = b_c;
		b_c = b_c->next;//阻塞指针指向下一个阻塞进程
		strcpy(p->state, "Ready");
		strcpy(p->reason, "Null");
		while (q->next)//插入就绪队列
			q = q->next;
		q->next = p;
		p->next = NULL;
		printf("\t* c1消费进程唤醒了!\n");
	}
}

void control() //处理器调度程序
{
	int rd;
	int num = 0;

	link p = ready;
	while (p) //统计就绪进程个数——ready链表的项数
	{
		num++;
		p = p->next;
	}
	printf("\t* 就绪进程个数为:%d\n", num);

	time_t t;
	srand((unsigned)time(&t));
	rd = rand() % num;//随机函数产生随机数


	cout <<"random:"<< rd << endl;

	if (rd == 1) {	//ready前两项交换
		cout << "交换就绪队列前两项" << endl;
		p = ready;
		ready = ready->next;
		ready->next = p;
		p->next = NULL;
		strcpy(ready->state, "Run");
		strcpy(ready->next->state, "Ready");
	}
	//不交换，运行就绪队列的第一个进程
	else
		strcpy(ready->state, "Run");
	flag = ready->breakp;	//保存断点
}

void processor() { //模拟处理器指令执行
	if (strcmp(ready->name, "Producer") == 0) //当前进程为生产者
		switch (flag) {
			case 0://produce
				printf("\t* 生产者生产了字符%d\n", space[sp]);	
				rec_p[rp1] = space[sp];
				rp1++;
				//rp1 = (rp1 + 1) % MAX;			
				flag++;
				ready->breakp = flag; //保存断点
				break;
			case 1: //p(empty)
				flag++;
				p(1);
				break;
			case 2: //put
				buffer[in] = rec_p[sp]; //放到缓冲区
				printf("\t* %d成功进入缓存区!\n", buffer[in]);	
				sp = (sp + 1) % MAX;
				in = (in + 1) % MAX;
				flag++;
				ready->breakp = flag; //保存断点
				break;
			case 3: //v(full)
				//flag++;
				printf("\t* 字符放入缓存区，生产空间-1\n");
				printf("\t* 生产进程断点置 0 \n");
				flag = 0;
				v(2);
				break;
			//case 4://goto00  

			//	ready->breakp = flag; //保存断点
			//	break;

	}
	else  //当前进程为消费者
		switch (flag) {
			case 0: //p(full)
				flag++;
				p(2);
				break;
			case 1: //get
				printf("\t* 消费者取字符!\n");
				temp = buffer[out];
				out = (out + 1) % MAX;
				flag++;
				ready->breakp = flag; //保存断点
				break;
			case 2: //v(empty)
				flag++;
				printf("\t* 字符离开缓存区，生产空间+1\n");

				v(1);
				break;
			case 3: //consume
				printf("\t* 消费了字符%d\n", temp);
				rec_c[rp2] = temp;//添加到消费记录
				//rp2 = (rp2 + 1) % MAX;
				rp2++;
				//flag++;
				printf("\t* 消费进程断点置 0 \n");
				flag = 0;
				ready->breakp = flag; //保存断点
				break;
			//case 4: //goto0
				//ready->breakp = flag; //保存断点
				//break;
	}
}

void print() {
	int i, j;

	printf("\n* 已生产:");
	for (j = 0; j <= rp1; j++)
		if (buffer[j] != 0)
			printf("%d ", rec_p[j]);
	printf("\nrec_p[%d]:%d", rp1, rec_p[rp1]);
	cout << endl<<rp1 << endl;
	printf("\n* 缓存区:");
	for (j = out; j <= in; j++)
		if (buffer[j] != 0)
			printf("%d ", buffer[j]);
		cout << endl << in - out << endl;
	printf("\n* 已消费:");
	for (j = 0; j <= rp2; j++)
		if (buffer[j] != 0)
			printf("%d ", rec_c[j]);
		cout << endl << rp2 << endl;
	printf("\n———————进程控制块的信息————————\n");
	printf("进程名\t\t状态\t等待原因\t断点\n");
	printf("%s\t%s\t%s\t\t%d\n\n", p1->name, p1->state, p1->reason, p1->breakp);
	printf("%s\t%s\t%s\t\t%d\n", c1->name, c1->state, c1->reason, c1->breakp);
	printf("———————————————————————\n");
	printf("1.继续 0.退出\n");
	//getchar();

}




void main() {
	printf("*生产者消费者模拟\n");
	printf("—————————\n");

	init();        //初始化
	while(1){
		printf("—————————模拟指令流程————————\n");
		control();      //处理器调度程序
		processor();    //模拟处理器指令执行
		print();        //输出显示各个信息
	}
}
