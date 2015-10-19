#include <stdio.h>
#include "pthread.h"
#include <iostream>
#include "math.h"
 
#define nThreads 4
void* func(void* arg);
struct range
{
	int* start;
	int* end;
};
using namespace std;
int main()
{
	int arSize;
	printf("Введите количество чисел в последовательности ");
	scanf("%d",&arSize);
	int* A;
	A = new int[arSize];
	//заполнение массива

	for (int i = 0; i < arSize; i++)
	{
		A[i] = i;
	}
	pthread_t arId[nThreads];
	for (int i = 0; i < nThreads; i++)
	{
		range* rI = new range;// создаем новый экземпляр переменнй
		rI->start = &(A[i*arSize/nThreads]); //к полям указателя на объект обращаемся через стрелку
		rI->end = &(A[i*arSize/nThreads+arSize/nThreads-1]);		
		pthread_create(&arId[i], NULL, func, rI);
	}

	int* Result[nThreads];
	for(int i = 0; i < nThreads; i++)
	{
		pthread_join(arId[i], (void**) &(Result[i]));
	}

	for(int i = 0; i < nThreads; i++)
	{
		for(int j = 0; j < 25; j++)
		{
			if (Result[i][j])
			cout << Result[i][j] << "  ";	
		}
		cout << endl;

	}
	return 0;
}
void* func(void* arg)
{
	int* B = new int[25]();
	int k = 0;
	range* pR = (range*) arg;
	for (int* i = pR->start; i < pR->end; i++)
	{
		if ( sqrt((float)*i) == (int) sqrt((float)*i) )
		{
			
			B[k] = (*i);
			k++;
		}
	}
	return B;
}