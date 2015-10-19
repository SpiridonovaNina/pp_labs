#include "pthread.h"
#include <iostream>
#include "math.h"
#include <mpi.h> 

int* func(void* arg);
struct range
{
	int* start;
	int* end;
};
using namespace std;
#define quantity 100;
int main()
{
	double startwtime = 0.0;
	double endwtime;
	MPI_Init(0,0) ; //инициализацияMPI 
	int rank,size; //узнаем ранг каждого процесса и их общее число
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ; 
	MPI_Comm_size(MPI_COMM_WORLD, &size) ; 
	int arSize = quantity;
	startwtime = MPI_Wtime();
	int kol=arSize/size ; //число элементов массива на каждый процесс
	int* A;
	A = new int[arSize];
	//заполнение массива

	for (int i = rank*kol; i < rank*kol+kol; i++)
	{
		A[i] = i;
	}
	
	range* rI = new range;// создаем новый экземпляр переменнй
	rI->start = &(A[rank *kol]); //к полям указателя на объект обращаемся через стрелку
	rI->end = &(A[rank*kol+kol-1]);	
	int* result = new int[kol]();
	result = func(rI);
	if (rank==0) 
	{
		for(int i = 0; i < kol-1; i++)
			{
				if (result[i])
					cout << result[i] << "  ";	
			}
			cout << endl;
	}
	if (rank!=0) MPI_Send(result,kol,MPI_INT,0,0,MPI_COMM_WORLD) ;
	if (rank==0) 
	{ 
		int* buf = new int[kol]() ; //для приема
		MPI_Status status ;  
		for (int i = 0 ; i < size-1 ; i++) 
		{
			MPI_Recv(buf, kol, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status) ; 
			for(int i = 0; i < kol-1; i++)
			{
				if (buf[i])
					cout << buf[i] << "  ";	
			}
			cout << endl;
		}
	}

	if (!rank)
	{
			endwtime = MPI_Wtime();
			cout << "time: " <<  endwtime-startwtime << endl;
	}
	
	MPI_Finalize();
	return 0;
	}
int* func(void* arg)
{
	range* pR = (range*) arg;
	int kol = pR->end - pR->start;
	int* B = new int[kol]();
	int k = 0;
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