#include<iostream>
#include<time.h>
using namespace std;

int main() {
	int hungry[5] ;
	int temp,random;
	int j;
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
			//hungry[i] = hungry[rd];
			//hungry[rd] = temp;
		}
		for (int i = 0; i < random; i++)
			cout << hungry[i] << "\t";
		cout << endl;
		_sleep(500);
	}
		return 0;
}