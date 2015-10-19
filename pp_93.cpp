#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <mpi.h>
void* readfile(void* arg);


int main(int argc, char* argv[])
{
	MPI_Init(0,0) ; //инициализацияMPI 
	int rank,size; //узнаем ранг каждого процесса и их общее число
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ; 
	MPI_Comm_size(MPI_COMM_WORLD, &size) ; 
	
	char* line_1 = new char[100]();
	char* line_2 = new char[100]();
	char line[25];
	int nLen;
	int nLen_1;
	int nLen_2;
	if(rank ==0)
	{
		FILE* fp = fopen("tag1.txt", "r");
		fgets(line,25, fp); 
	}

		
	if(rank ==1)
	{
		FILE* fp = fopen("tag2.txt", "r");
		fgets(line_1,25, fp); 
		MPI_Send(line_1,25,MPI_CHAR,0,1,MPI_COMM_WORLD) ;

	}
	
	if(rank ==2)
	{
		FILE* fp = fopen("tag3.txt", "r");
		fgets(line_2,25, fp); 
		MPI_Send(line_2,25,MPI_CHAR,0,2,MPI_COMM_WORLD) ;
	}
	
	if(rank ==0)
	{
		printf("Потоки меняются символами с одинаковыми индексами,если хотя бы один из этих символов есть в общей строке\n");
		MPI_Status status ; 
		MPI_Recv(line_1, 25, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status) ; 
		MPI_Recv(line_2, 25, MPI_CHAR, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status) ; 
		nLen = strlen(line);
		nLen_1 = strlen(line_1);
		nLen_2 = strlen(line_2);
		printf("%s\n", line);
		printf("%s\n", line_1);
		printf("%s\n", line_2);
	
	int less_len;
	if(nLen_1 <  nLen_2)
		less_len = nLen_1;
	else less_len = nLen_2;

	int consilience = 0;
	for (int i = 0; i < less_len; i++)
	{
		for(int m = 0; m < nLen; m ++)
		{
			if((line_1[i] == line[m]) || (line_2[i] == line[m]))
			{consilience = 1;}
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

	printf("измененные строки\n");
	
	printf("%s\n", line_1);
	printf("%s\n", line_2);
	}
	
	MPI_Finalize();
	return 0;
}



