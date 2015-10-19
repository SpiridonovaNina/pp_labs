#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int buf[100];
int front = 0; // индекс для чтения из буфера
int rear = 0; // индексь для записи в буфер
sem_t empty; // семафор отображающий сколько еще может встать в очередь покупателей
sem_t full ; //количество человек, которые кассиру необходимо обслужить
pthread_mutex_t mutexB; //мьютекс для операции записи
pthread_mutex_t mutexC; //мьютекс для операции Чтения
void *cashier(void *param);
void *buyer(void *param);

int main(int argc, char* argv[])
{
	int i ; 
	//инициализация мьютексов и семафоров
	pthread_mutex_init(&mutexB, NULL) ; 
	pthread_mutex_init(&mutexC, NULL) ; 
	sem_init(&empty, 0, 100) ; //количество свободных ячеек равно100
	sem_init(&full, 0, 100);
	//запуск buyer
	pthread_t threadB[1] ; 
	for (i = 0 ; i < 1 ; i++) 
	pthread_create(&threadB[i],NULL,buyer,NULL) ; 
	//запуск cashier
	pthread_t threadC[2] ; 
	for (i = 0 ; i < 2 ; i++) 
	{
		int *p = new int;
		*p = i;
		pthread_create(&threadC[i],NULL,cashier,p) ; 
	}
	for(int i = 0; i < 1; i++)
	pthread_join(threadB[i],NULL);
	for(int i = 0; i < 2; i++)
	pthread_join(threadC[i],NULL);
	return 0;
}

//стартовая функция потоков– покупателей 
void *buyer(void *param) 
{ 
	int  i ; 
	int data = 0;
	int sval;
	sem_getvalue(&empty,&sval);
	while (sval > 0) 
	{ 
		//создать элемент для буфера
		data = rand()%10 ; 
		//поместить элемент в буфер
		pthread_mutex_lock(&mutexB) ; //защита операции записи
		sem_wait(&empty) ; //количество человек которые еще смогут встать в очередь уменьшить на единицу
		sem_getvalue (&empty,&sval);
		buf[rear] = data ; 
		rear = rear+1 ; 
		pthread_mutex_unlock(&mutexB) ; 
	} 
	printf ("buyers ends/n");
	return 0;
} 

//стартовая функция потоков– кассиров 
void *cashier(void *param) 
{
	int* id = ((int*) param);
	int result ; 
	int sval_2;
	//sem_getvalue (&full,&sval_2);
	while (1) 
	{ 
		//извлечь элемент из буфера
		pthread_mutex_lock(&mutexC) ; //защита операции чтения
		sem_wait(&full) ; //количество человек, которые кассиру необходимо обслужить уменьшить на единицу
		sem_getvalue (&full,&sval_2);
		result = buf[front] ;
		front = front+1; 
		pthread_mutex_unlock(&mutexC) ; 
		//обработать полученный элемент 
		if (*id == 0)
			printf ("+%d!\n", result);
		else
			printf ("-%d!\n", result);
	} 
	printf ("cashier ends/n");
	return 0;
}