#include<iostream>
#include<stdlib.h>
using namespace std;

#define N 100
int count = 0;

void producer(){
	int item;
	while (1){
		item = produce_item();
		if (count == N)sleep();
		
	}
}