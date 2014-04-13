#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include<time.h>

#define NUM 20
int queue[NUM];
int p = 0,c = 0;//记录动态数组下标位置
sem_t blank_number, product_number;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

//生产者函数
void *producer(const int i)
{
	int j;
	time_t t;
	srand((unsigned) time(&t));//初始化随机数种子
	while(1)
	{
		sem_wait(&blank_number);
		pthread_mutex_lock(&counter_mutex);
		sleep(2);
		queue[p]=rand()%NUM+1;//1到20随机数
		pthread_mutex_unlock(&counter_mutex);
		sem_post(&product_number);
		printf("生产者 %d 号的线程标识符: %u ,",i+1,((unsigned int)pthread_self()));
		printf("生产出 %d\n",queue[p]);
		printf("缓冲区当前状态：**********************************\n");
		printf("编号:");
		for(j = 0;j <NUM;j ++)
		{
			printf("%4d",j+1);
		}
		printf("\n数值:");
		for(j = 0;j < NUM;j ++)
		{
			printf("%4d",queue[j]);
		}
		printf("\n指针:");
		for(j = 0;j < NUM;j++)
		{
			if(j != p)
				printf("    ");
			else
				printf("   ↑");
		}
		printf("\n");
		p=(p + 1)%NUM;//p后移一位
		sleep(rand() % 5);//随机阻塞0-4秒
	}	
	return NULL;
}

//生产线程入口函数
void *producer_thread_fun(void *arg)
{
	int realarg = (int)arg;//强制类型转换参数
	producer(realarg);
	return NULL;
}

//消费者函数
void *consumer(const int i)
{
	int j;
	time_t t;
	srand((unsigned) time(&t));
	while(1)
	{
		sem_wait(&product_number);
		pthread_mutex_lock(&counter_mutex);
		sleep(2);
		printf("消费者 %d 号的线程标识符: %u ,",i+1,((unsigned int)pthread_self()));
		printf("消费了 %d\n",queue[c]);
		queue[c] = 0;//空位置 0 填充
		pthread_mutex_unlock(&counter_mutex);
		sem_post(&blank_number);
		printf("缓冲区当前状态：**********************************\n");
		printf("编号:");
		for(j = 0;j <NUM;j ++)
		{
			printf("%4d",j+1);
		}
		printf("\n数值:");
		for(j = 0;j < NUM;j ++)
		{
			printf("%4d",queue[j]);
		}	
		printf("\n指针:");
		for(j = 0;j < NUM;j++)
		{
			if(j != c)
				printf("    ");
			else
				printf("   ▲");
		}
		printf("\n");
		c = (c + 1)%NUM;
		sleep(rand() % 5);
	}
}

//消费线程入口函数
void *consumer_thread_fun(void *arg)
{
	int realarg = (int)arg;
	consumer(realarg);
	return NULL;
}
//MAIN
int main(int argc, char *argv[]) 
{
	int i;
	pthread_t pid[100],cid[100];
	int producer_num,consumer_num;
	printf("********多进程同步解决生产者--消费者问题********\n");
	printf("************************************************\n");
	sleep(1);
	for(i = 0;i < NUM;i ++)//初始化缓冲区
	{
		queue[i] = 0;
	}
	printf("请输入生产者数量:");
	scanf("%d",&producer_num);
	while(producer_num > 100 ||producer_num < 1)
	{
		printf("生产者数量必须在1--100之间\n");
		printf("请重新输入:");
		scanf("%d",&producer_num);
	}

	printf("请输入消费者数量:");
	scanf("%d",&consumer_num);
	while(consumer_num > 100 ||consumer_num < 1)
	{
		printf("消费者数量必须在1--100之间\n");
		printf("请重新输入:");
		scanf("%d",&consumer_num);
	}

	for(i = 0;i < producer_num;i ++)
	{
		pthread_t pid[i];
	}
	for(i = 0;i < consumer_num;i ++)
	{
		pthread_t cid[i];
	}

	//初始化semaphore ;pshared参数为0表示信号量用于同一进程的线程间同步
	sem_init(&blank_number, 0, NUM);
	sem_init(&product_number, 0, 0);

	for(i = 0;i < producer_num;i ++)//创建线程
	{
		pthread_create(&pid[0]+i, NULL, producer_thread_fun, (void *)i);//i强制将i转换为void*类型，便于传参
	}
	for(i = 0;i < consumer_num;i ++)
	{
		pthread_create(&cid[0]+i, NULL, consumer_thread_fun, (void *)i);
	}
	for(i = 0;i < producer_num;i ++)//主线程等待所有线程撤销
	{
		pthread_join(pid[i], NULL);
	}
	for(i = 0;i < consumer_num;i ++)
	{
		pthread_join(cid[i], NULL);
	}
		
	sem_destroy(&blank_number);//释放semaphore资源
	sem_destroy(&product_number);
	return 0;
}
