#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int buf[100];
int front = 0; // индекс для чтения из буфера
int rear = 0; // индексь для записи в буфер

int count = 0 ; //количество занятых ячеек буфера
pthread_mutex_t mutex; //мьютекс для условных переменных
/*поток-покупатель блокируется этой условной переменной, когда количество
занятых ячеек становится равно100*/
pthread_cond_t not_full ; 
/*поток-кассир блокируется этой условной переменной, когда количество
занятых ячеек становится равно0*/ 
pthread_cond_t not_empty ;

void *cashier(void *param);
void *buyer(void *param);
int main(int argc, char* argv[])
{
	int i ; 
	//инициализация мьютексов и семафоров
	pthread_mutex_init(&mutex, NULL) ; 
	pthread_cond_init(&not_full, NULL) ; 
	pthread_cond_init(&not_empty, NULL) ;
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
	while (1) 
	{ 
		//создать элемент для буфера
		data = rand()%10 ; 
		//поместить элемент в буфер
		pthread_mutex_lock(&mutex) ; //заснуть, если количество занятых ячеек равно сто
		while (count == 100 ) pthread_cond_wait(&not_full,&mutex);
		buf[rear] = data ; 
		rear = rear+1 ; 
		count++ ; //появилась занятая ячейка
		pthread_mutex_unlock(&mutex) ; 
		//разбудить потоки-кассиры после добавления элемента в буфер
		pthread_cond_broadcast(&not_empty) ; 
	} 
	printf ("buyers ends/n");
	return 0;
} 

//стартовая функция потоков– кассиров 
void *cashier(void *param) 
{
	int* id = ((int*) param);
	int result ; 
	while (1) 
	{ 
		//извлечь элемент из буфера
		pthread_mutex_lock(&mutex) ; //защита операции чтения
		//заснуть, если количество занятых ячеек равно нулю
		while (count == 0 ) pthread_cond_wait(&not_empty, &mutex) ;
		//изъятие из общего буфера– начало критической секции
		result = buf[front] ;
		front = front+1; 
		count-- ; //число человек, которых нужно обслужить кассиру уменьшается на 1
		//конец критической секции
		pthread_mutex_unlock(&mutex) ; 
		//разбудить потоки-писатели после получения элемента из буфера
		pthread_cond_broadcast(&not_full) ;
		//обработать полученный элемент 
		if (*id == 0)
			printf ("+%d!\n", result);
		else
			printf ("-%d!\n", result);
	} 
	printf ("cashier ends/n");
	return 0;
}