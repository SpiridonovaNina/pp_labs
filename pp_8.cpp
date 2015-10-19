#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>


void* readfile(void* arg);
pthread_mutex_t mutex;
pthread_barrier_t barr ; //барьер

int main(int argc, char* argv[])
{
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barr, NULL, 3) ; //инициализация барьера
	
	char* line_1 = new char[100]();
	char* line_2 = new char[100]();
	char line[100];
	
	printf("Введите симвлы, без повторов\n\n");
	gets(line);
	int nLen = strlen(line);

	pthread_t thread[2]; 
	pthread_create(&thread[0], NULL, readfile,line_1) ;
	pthread_create(&thread[1], NULL, readfile,line_2) ;
	pthread_barrier_wait(&barr);
	
	int nLen_1 = strlen(line_1);
	int nLen_2 = strlen(line_2);

	printf("Потоки меняются символами с одинаковыми индексами,если хотя бы один из этих символов есть в общей строке\n");
	int less_len;
	if(line_1 <  line_2)
		less_len = nLen_1;
	else less_len = nLen_2;

	int consilience = 0;
	for (int i = 0; i < less_len; i++)
	{
		for(int m = 0; m < nLen; m ++)
		{
			if((line_1[i] == line[m]) || (line_2[i] == line[m]))
			consilience = 1;
		}
		
		if(consilience == 1)
		{
		char temp;
		temp = line_1[i];
		line_1[i] = line_2[i];
		line_2[i] = temp;
		}
		consilience = 0;
	}


	printf("%s", line_1);
	printf("\n");
	printf("%s", line_2);
	printf("\n");;
	return 0;
}

void* readfile(void* arg)
{
	char* text = (char*)arg;
	pthread_mutex_lock(&mutex);
	printf("Введите текст\n");
	gets(text);
	pthread_mutex_unlock(&mutex);
	pthread_barrier_wait(&barr);
	return 0;
}
