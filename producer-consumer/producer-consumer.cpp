#include<iostream>
using namespace std;

#define N 20;
int mutex = 1;
int empty = N;
int	full = 0;
int buffer[20];
int share[20];
int item = 0;


int produce_item(){
	
	while (buffer[item] != 0){
		item = (item++) % N;
	}
	return item;
}
int consume_item(int item){
	buffer[item] = 0;
	return 1;
}

void insert_item(int item){
	buffer[item] = 1;
	cout << "生产了一个产品" <<buffer[item]<<endl;
}
int remove_item(){
	cout<<"消费了一个产品"<<buffer[item]<<endl;
	return buffer[item];
}




void down(int f){
	f--;
	if (f == 0)
		consumer();

}
void up(int f){

}

void producer(){
	//int item;

	while(1){
		item = produce_item();
		down(empty);
		down(mutex);
		insert_item(item);
		up(mutex);
		up(full);

	}
}

void consumer(){
	//int item;

	while(1){
		down(full);
		down(mutex);
		item = remove_item();
		up(mutex);
		up(empty);
		consume_item(item);
	}
}
	



int main(){
	producer();
	consumer();
	return 0;
	
}