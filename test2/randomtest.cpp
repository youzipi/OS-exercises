#include<iostream>
#include<time.h>
using namespace std;

int main() {
	int hungry[5] = {1,2,3,4,5};
	int a[5] = {0,0,0,0,0};
	int rd, random;
	int i,j;
	while (1) {
		time_t t;
		srand((unsigned)time(&t));
		random = rand() % 5 + 1;//����������������
		cout << random << "λ��ѧ�Ҷ���" << endl;
		rd = random;
		j = 0;
		while (rd) {
			time_t t;
			srand((unsigned)time(&t));
			i = rand() % rd;
			a[j] = hungry[i];
			j++;
			hungry[i] = hungry[rd - 1];
			rd--;
			hungry[rd] = rd + 1;
		}
		for (int i = 0; i < random; i++)
			cout << a[i] << "\t";
		cout << endl;
		_sleep(1500);
	}
	return 0;
}