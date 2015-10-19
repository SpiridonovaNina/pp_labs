#include <cmath>
#include <mpi.h>
#include "pthread.h"
#include <stdio.h>

int** determinant(int start, int end, int **mass);
int calculate_matrix_third(int **matrix);
int** change_matrix(int **mass, int * mass_b, int N);
int GetDeterm(int **mass);

struct equations
{
	int **mass;
	int *mass_b;
	int column;
};
int GetChangeDeterm(equations* arg);

int main(int argc, char* argv[])
{
	MPI_Init(0,0) ; //инициализацияMPI
	int rank,size; //узнаем ранг каждого процесса и их общее число
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ; 
	MPI_Comm_size(MPI_COMM_WORLD, &size) ;

	int **mass;
	int **mass_third;
	int mass_det[4];// массив из определителей
	int det;
	int * mass_b = new int[4];

		mass = new int* [4];
		for (int i = 0; i < 4; i++)
			mass[i] = new int[4];

	FILE* fp = fopen("matrix_92.txt", "rt");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		fscanf(fp,"%d",&mass[i][j]);
		}
		fscanf(fp,"%d",&mass_b[i]);
	}

	// считаем определитель матрицы
		det = GetDeterm(mass);
	
	//считаем определитель дельта1,дельта2,дельта3,дельта4
	
		int det_t;
		equations* parametrs = new equations;
		parametrs->mass = mass;
		parametrs->mass_b = mass_b;
		parametrs->column = rank;
		det_t = GetChangeDeterm(parametrs);

			printf("X_%d = %d/%d = %f \n", rank, det_t,det,(float)det_t/det);

			if(rank == 0)
	{

		printf("Вы ввели матрицу:\n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%3d   ", mass[i][j]);

			}
			printf("\n");
		}

		printf("Вы ввели массив b:\n");
		for(int i = 0; i < 4; i++)
		{
			printf("%d \n", mass_b[i]);
		}
		printf("Главный определитель равен = %d\n", det);
	}


	MPI_Finalize();
		
	return 0;
}

//найдем матрицу алгеброического дополнения
int** determinant(int start, int end, int **mass)
{
	int i;
	int j;
	int n = 0;
	int m = 0;
	int** mass_third = new int*[3];
	for (i = 0; i < 3; i++)
	{
		mass_third[i] = new int[3];
	}

	i = 0; 
	j = 0;
	for(i = 0; i < 4; i++)
	{
		if ( i != start)
		{
			for(j = 0; j < 4; j++)
			{
				if( j != end)

				{
					mass_third[m][n] = mass[i][j];
					if( n==2 )
					{
						m++;
						n=0;
					}
					else
						n++;
				}

			}
		}

	}
	return mass_third;
}

int calculate_matrix_third(int **matrix)
{
	int **matrix_add;
	matrix_add = new int *[5];
	for(int i = 0; i < 5; i++)
		matrix_add[i] = new int [3];
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			matrix_add[i][j] = matrix[i][j];
		}

		int k = 0;
		for( int i = 3; i < 5; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				matrix_add[i][j] = matrix[k][j];

			}
			k ++;
		}

		int result = 0; 
		int result_1 = 0;
		int result_mult = 1;
		k = 0;
		for(int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				result_mult = result_mult*matrix_add[i+k][i];
			}
			result_1 = result_1 + result_mult;
			k++;
			result_mult = 1;
		}

		k = 0;
		for(int j = 0; j < 3; j++)
		{
			for (int i = 2; i > -1; i--)
			{
				result_mult = result_mult*matrix_add[2-j+k][i];
				k++;
			}
			result = result + result_mult;
			k = 0;
			result_mult = 1;
		}
		result = result_1 - result;
		return result;

}

int** change_matrix(int **mass, int * mass_b, int N)
{
	int **mass_change = new int*[4];
	for(int i = 0; i < 4; i++)
	{
		mass_change[i] = new int[4];
	}

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			mass_change[i][j]=mass[i][j];
		}
		for(int i = 0; i < 4; i++)
		{
			mass_change[i][N] = mass_b[i];
		}

		return	mass_change;
}

int GetDeterm(int **mass)
{
	int det = 0;
	for(int i = 0; i < 4; i++)
	{
		int** t = determinant(0, i, mass);
		int opred_1 = calculate_matrix_third(t);
		det = det + pow(-1,(double)2+i)* mass[0][i] * opred_1;
	}
	return det;
}
int GetChangeDeterm(equations* arg)
{
	equations *parametrs = (equations *) arg;
	
	int **mass_1 = change_matrix(parametrs->mass,parametrs->mass_b,parametrs->column);
	int det = GetDeterm(mass_1);
	for (int i = 0; i < 4; i++)
	{
		delete [] mass_1[i];
	}
	delete [] mass_1;

	delete parametrs;

	return det;
}
