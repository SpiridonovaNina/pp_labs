#include <stdio.h>
#include <cmath>
#include "pthread.h"


int** determinant(int start, int end, int **mass);
int calculate_matrix_third(int **matrix);
int** change_matrix(int **mass, int * mass_b, int N);
int GetDeterm(int **mass);
void *GetChangeDeterm(void* arg);
struct equations
{
	int **mass;
	int *mass_b;
	int column;
};
pthread_mutex_t mutex ; //двоичный семафор
struct rezult
{
	int det;
	int i;
	rezult *next;
};
rezult *head; //указатель на первый элемент очереди
rezult *newrez; //указатель/для создания новых элементов очереди
rezult *rez; //указатель на текущий последний элемент очереди

int main(int argc, char* argv[])
{
	int height;
	int **mass;
	int **mass_third;

	mass = new int* [4];
	for (int i = 0; i < 4; i++)
		mass[i] = new int[4];


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("Введите элемент матрицы [%d] [%d] : ", i,j);
			scanf("%d", &(mass[i][j]));

		}
	}

	printf("Вы ввели матрицу:\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%3d   ", mass[i][j]);

		}
		printf("\n");
	}


	int * mass_b = new int[4];

	for(int i = 0; i < 4; i++)
	{
		printf("Введите b%d ", i);
		scanf("%d", &(mass_b[i]));
	}


	// считаем определитель матрицы
	int det = GetDeterm(mass);

	printf("Главный определитель равен = %d\n", det);
	

	//считаем определитель дельта1,дельта2,дельта3,дельта4

	head = new rezult;
	rez = head; //создание первого элемента очереди
	pthread_mutex_init(&mutex, NULL); //инициализация двоичного семафора
	//идентификаторы для дочерних потоков

	pthread_t arId[4];
	for(int i = 0; i < 4; i++)
	{
		equations* parametrs = new equations;
		parametrs->mass = mass;
		parametrs->mass_b = mass_b;
		parametrs->column = i;
		pthread_create(&(arId[i]),NULL,GetChangeDeterm,(void*)parametrs);

	}

	for(int i = 0; i < 4; i++)
	{
		pthread_join(arId[i], NULL);
	}
	
	//for(int i = 0; i < 4; i++)
	//	printf("X_%d = %d/%d = %f \n", i, mass_det[i],det,(float)mass_det[i]/det);

	rez = head->next ; 
	while (rez!=NULL) 
	{ 
		printf("X_%d = %d/%d = %f \n", rez->i, rez->det,det,(float)rez->det/det) ;
		rez = rez->next; 
	}


	for (int i = 0; i < 4; i++)
		delete [] mass[i];
	delete [] mass;
	delete [] mass_b;

		
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
void *GetChangeDeterm(void* arg)
{
	equations *parametrs = (equations *) arg;
	
	int **mass_1 = change_matrix(parametrs->mass,parametrs->mass_b,parametrs->column);
	int det = GetDeterm(mass_1);
	
	pthread_mutex_lock(&mutex); /*протокол входа в КС: закрыть
	двоичный семафор*/ 
	//начало критической секции– запись результата в очередь
	newrez = new rezult;
	newrez->det = det;
	newrez->i = parametrs->column;
	newrez->next = NULL ; 
	rez->next = newrez ; 
	rez = newrez ; 
	pthread_mutex_unlock(&mutex); /*протокол выхода из КС: 
	открыть двоичный семафор*/

	for (int i = 0; i < 4; i++)
	{
		delete [] mass_1[i];
	}
	

	delete [] mass_1;

	delete parametrs;
	return NULL;
}
