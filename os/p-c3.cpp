#include<iostream>
#include<stdlib.h>
#include<windows.h>
using namespace std;

typedef HANDLE Semaphore;
#define P(S) WaitForSingleObject(S, INFINITE)
#define V(S) ReleaseSemaphore(S,1,NULL)
#define  N 1000
#define PRODUCER_NUM 2
#define CONSUMER_NUM 2
#define BUFFER_NUM 10

int thing[10] = {1,2,3,4,5,6,7,8,9,0};
struct buffer{
	int product[BUFFER_NUM];
	int start, end;
}buf;

HANDLE empty,full,mutex;

DWORD WINAPI consumer(LPVOID para){
	int i = *(int *)para;
	int t;
	cout << "hehe:" <<i<< endl;
	Sleep(1000);
	while (1){
		cout << "consuming " << i << endl;
		P(full);
		P(mutex);
		t = buf.start;
		buf.start = (buf.start + 1) % BUFFER_NUM;
		V(mutex);
		cout << "consumed " << i << t << thing[buf.product[t]] << endl;
		Sleep(1000 * rand() % 10 + 110);
		V(empty);
	}
	return 0;
}

DWORD WINAPI producer(LPVOID para){
	int i = *(int *)para - CONSUMER_NUM;
	int t;
	int product;

	cout << "producer " << i << endl;
	Sleep(1000);
	while (1){
		cout << "producer " << i << "working " << endl;
		P(empty);
		P(mutex);
		t = buf.end;
		buf.end = (buf.end + 1) % BUFFER_NUM;
		V(mutex);
		cout << "producing" << i << t << thing[buf.product[t]] << endl;
		buf.product[t] = product;
		Sleep(1000 / 2 * rand() % 10 + 110);
		cout << "producted " << i << t << thing[buf.product[t]] << endl;
		V(full);

	}
	return 0;
}

int main(){
	HANDLE hThread[CONSUMER_NUM + PRODUCER_NUM];
	DWORD tid;
	int i = 0;
	mutex = CreateSemaphore(NULL, BUFFER_NUM, BUFFER_NUM, 0);
	empty = CreateSemaphore(NULL, BUFFER_NUM, BUFFER_NUM, 0);
	full = CreateSemaphore(NULL, BUFFER_NUM, BUFFER_NUM, 0);
	if (!empty || !full || !mutex){
		cout << "ERROR!" << endl;
		return -1;
	}

	int totalThreads = CONSUMER_NUM + PRODUCER_NUM;
	cout << "begin" << endl;
	for (i = 0; i < CONSUMER_NUM; i++){
		hThread[i] = CreateThread(NULL, 0, consumer, &i, 0, &tid);
		if (hThread[i])
			P(hThread[i], 10);
	}
	cout << "producer..." << endl;
	for (; i<totalThreads; i++){
		hThread[i] = CreateThread(NULL, 0, producer, &i, 0, &tid);
		if (hThread[i])   WaitForSingleObject(hThread[i], 10);
	}

	//   生产者和消费者的执行   
	WaitForMultipleObjects(totalThreads, hThread, TRUE, INFINITE);
	return   0;

}