#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"


struct Student
{
	char name[10];
	int number_marks;
	int marks[6];
};
struct group
{
	int number_group;
	int studentCount;
	Student students[30];
	group *next;
};

struct find
{
	int i;			//номер потока
	int number;		// номер группы
	char Sname[10];	//имя студента
};

void* addGroup(void* arg);
void* deleteGroup(void* arg);
void* find_s_for_del(void* arg);
void* find_s_for_setmark(void* arg);
void* find_s_for_set_middle_mark(void* arg);
void getStudent(group *st, int i);
void setMarks(Student& st);
void deleteSdent(group *st, int del);
void setMarks(Student &st);
float Get_average_score(Student &st);

int printMenu();

group *head; //указатель на первый элемент списка
group *tail; //указатель на текущий последний элемент списка
int count_group = 0;
void* find_student(void* arg);

pthread_rwlock_t lock ;  //блокировка чтения-записи
pthread_rwlock_t lock_Del_G ;  //блокировка чтения-записи

pthread_mutex_t mutex ; //двоичный семафор

int main(int argc, char* argv[])
{
	pthread_rwlock_init (&lock,NULL);
	pthread_mutex_init(&mutex, NULL);

	head = NULL;
	tail = NULL;
	int menu = 0;
	do
	{
		menu = printMenu();
		printf("Вы выбрали пункт меню %d\n",menu);
		switch(menu)
		{
		case 1:
			{
				system("clear");
				tail = head;
				while (tail != NULL)
				{
					printf("\nНомер группы :%d\n",tail->number_group );
					for(int i = 0; i < tail->studentCount; i++)
					{
						printf("\nСтудент :%s\n",tail->students[i].name);
						printf("Оценки:");
						for(int j = 0; j < tail->students[i].number_marks; j++)
							printf("%d", tail->students[i].marks[j]);
					}
					printf("\n");
					tail = tail->next;
				}
				
			}
			break;
		case 2:
			{
				system("cls");
				char key[2];
				key[0] = '1';
				do
				{
					pthread_t thread_G[1];
					pthread_create(&thread_G[0], NULL, addGroup,NULL);
					pthread_join (thread_G[0],NULL);
					printf ("Добавить еще группу? Если нет, введите 0.\n");
					fflush(NULL);
					scanf("%s", &key);

				}while (key[0] != '0');
				
			
			}
			break;
		case 3:
			{
				char key = '1';
				do
				{
					find* fs = new find;
					printf ("Выберете номер группы в которую вы хотите добавить студента \n");
					scanf("%d",&(fs->number));
					pthread_t thread_S[2];
					fs->i = 1;
					pthread_create(&thread_S[0], NULL,find_student,fs);
					find* f = new find;
					f->i = 2;
					f->number = fs->number;
					pthread_create(&thread_S[1], NULL,find_student,f);
					pthread_join (thread_S[0],NULL);
					pthread_join (thread_S[1],NULL);
					printf ("Добавить еще группу? Если нет, введите 0.\n");
					scanf("%c", &key);

				}while (key != '0');

			}
			break;
		case 4:
			{
				find* mk = new find;
				find* m = new find;
				printf ("Введите имя студента чтобы добавить ему оценку \n");
				scanf("%s",&mk->Sname);
				mk->i = 1;
				pthread_t thread_S_M[2];
				pthread_create(&thread_S_M[0], NULL,find_s_for_setmark,mk);
				m->i = 2;
				for(int i = 0 ; i < 10; i++)
					m->Sname[i] = mk->Sname[i];
				pthread_create(&thread_S_M[1], NULL,find_s_for_setmark,m);
				pthread_join (thread_S_M[0],NULL);	
				pthread_join (thread_S_M[1],NULL);	
			}
			break;
		case 5:
			{
				int number;
				printf(" введите номер группы которую вы хотите удалить");
				scanf("%d",&number);
				tail = head;
				if(tail->number_group != number)
				{
					tail = tail->next;
				}
				pthread_t thread_Del[1];
				pthread_create(&thread_Del[0], NULL, deleteGroup,tail);
				pthread_join (thread_Del[0],NULL);		
			}
			break;
		case 6:
			{
				find* ds = new find;
				find* d = new find;
				printf ("Введите имя студента чтобы  удалить его из группы \n");
				scanf("%s",&ds->Sname);
				ds->i = 1;
				pthread_t thread_S_D[2];
				pthread_create(&thread_S_D[0], NULL,find_s_for_del,ds);
				d->i = 2;
				for(int i = 0 ; i < 10; i++)
					d->Sname[i] = ds->Sname[i];
				pthread_create(&thread_S_D[1], NULL,find_s_for_del,d);
				pthread_join (thread_S_D[0],NULL);

				
			}
			break;
			case 7:
			{
				find* smm = new find;
				find* mm = new find;
				printf ("Введите имя студента, чтобы узнать его среднюю оценку\n");
				scanf("%s",&smm->Sname);
				smm->i = 1;
				pthread_t thread_S_D[2];
				pthread_create(&thread_S_D[0], NULL,find_s_for_set_middle_mark,smm);
				mm->i = 2;
				for(int i = 0 ; i < 10; i++)
					mm->Sname[i] = smm->Sname[i];
				pthread_create(&thread_S_D[1], NULL,find_s_for_set_middle_mark,mm);
				pthread_join (thread_S_D[0],NULL);
				pthread_join (thread_S_D[1],NULL);
			}

			break;
		case 0:
			break;
		default:
			printf("Неверный пункт меню\n");

		}
		
	} while (menu != 0);
	
	return 0;
	
}
int printMenu()
{

	printf("\n\nМеню программы\n");
	printf("1.Посмотреть, какие есть группы\n");
	printf ("2.Добавить ноновую  группу  и составить  для  нее  список  студентов\n");
	printf ("3.Добавить студента в группу \n");
	printf ("4.Добавить оценку  в  список  оценок  произвольного  студента\n");
	printf ("5.Удалить группу\n");
	printf("6.Удалить студента из группы\n");
	printf("7.Определить среднийбалл произвольного студента\n");
	printf ("_ _ _ _ _ _\n");
	printf ("0. Выход\n");

	int m;
	scanf("%d", &m);
	getchar();

	return m ;

}
//поток,  добавляющий  новую  группу  и составляющий  для  нее  список  студентов
void* addGroup(void* arg)
{
	group* newGroup = new group;
	newGroup->next = NULL;

	printf("Введите номер группы \n");
	scanf("%d", &newGroup->number_group);
	printf("Введите количество студентов в группе \n");
	scanf("%d", &newGroup->studentCount);
	for(int i = 0; i < newGroup->studentCount; i++)
	{
		 getStudent(newGroup, i);
	}
	
	// TODO: блокировка
	if (tail != NULL)
	{
		tail->next = newGroup;
		tail = tail->next;
	}
	else
	{
		head = newGroup;
		tail = newGroup;
	}
	// TODO: разблокировка
	count_group++;
	return 0;
}

void getStudent(group *st, int i)
{
	
	printf("\nВведите имя студента:");
	scanf("%s", st->students[i].name);
	st->students[i].number_marks = 0;
	setMarks(st->students[i]);
	
}

void setMarks(Student &st)
{
	int number;
	printf("Введите количество оценок которые вы хотите добавить: ");
	scanf("%d",&number);
	for(int j = st.number_marks; j < st.number_marks + number; j++)
	{
		printf("\nВведите оценку студента:");
		scanf("%d", &(st.marks[j]));
	}
	st.number_marks = st.number_marks + number;
}

void* deleteGroup(void* arg)
{
	group*sp = (group*) arg;
	// Если УЭ – это начало списка, то
	if (sp == head)
		{
			pthread_rwlock_wrlock (&lock_Del_G);
			// НС = Следующий за НС.
			head = head->next;
			// Обнулить связь УЭ со след. элементом.
			sp->next = NULL;
			// Выйти из функции, вернув НС.
			pthread_rwlock_unlock (&lock_Del_G);
		}
	else
	{
		pthread_rwlock_wrlock (&lock_Del_G);
		// Установить ПЭ равным НС.
		group *p = head;
		// Цикл пока у ПЭ есть след. элемент
		while (p->next != NULL)
		{
			// Если след. для ПЭ – это УЭ, то
			if (p->next == sp)
			{
				// Остановить цикл
				break;
			}
			// Установить ПЭ равным следующему за ПЭ элементу
			p = p->next;
		}
		// Связать ПЭ с элементом, следующим за УЭ
		p->next = sp->next;
		// Обнулить связь УЭ со след. элементом
		sp->next = NULL;
		pthread_rwlock_unlock (&lock_Del_G);
	}
	return 0;
}

void deleteSdent(group *st, int del)
{
	Student newStudents[30];
	for(int i = 0; i < del; i++)
		newStudents[i] = st->students[i];

	for(int i = del+1; i < st->studentCount; i++)
		newStudents[i-1] = st->students[i];
	st->studentCount = st->studentCount -1;
	for(int i = 0; i < st->studentCount; i++)
	st->students[i] = newStudents[i];
	
}

float Get_average_score(Student &st)
{
	float  sum = 0;
	float res = 0;
	for(int i = 0; i < st.number_marks; i++)
		sum = sum + (float)st.marks[i];
	res = sum/st.number_marks;
		return res;
}

void* find_student(void* arg)
{


		find* fs = (find*) arg; 
		int count = 0;
		if(fs->i == 1)
		{

			pthread_rwlock_wrlock (&lock);

			while( count < count_group/2)
			{
				tail = head;
				if(tail->number_group != fs->number)
				{
					tail = tail->next;
					count++;
				}
				else
				{
					tail->studentCount = tail->studentCount + 1;
					getStudent(tail, tail->studentCount-1);
					count++;
				}
			}
			pthread_rwlock_unlock (&lock);
		}
		else
		{
			pthread_rwlock_wrlock (&lock);
			tail = head;
			count = count_group/2;
				for( int j = 0; j < count; j++)
					tail = tail->next;
			while(count < count_group)
			{
				if(tail->number_group != fs->number)
				{
					tail = tail->next;
					count++;
				}
				else
				{
					tail->studentCount = tail->studentCount + 1;
					getStudent(tail, tail->studentCount-1);
					tail = tail->next;
					count++;
				}
			}
			pthread_rwlock_unlock (&lock);

		}

	return 0;
 }

void* find_s_for_del(void* arg)
{

	find* ds = (find*) arg;
	tail = head;
	int count = tail->studentCount/2;

	if(ds->i == 1)
	{
		tail = head;
		while (tail != NULL)
		{
		
			for(int i = 0; i < count; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					deleteSdent(tail , i);
				}
			}
			tail = tail->next;
		}	
	}
	else
	{
		tail = head;
		while (tail != NULL)
		{

			for(int i = count; i < tail->studentCount; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					deleteSdent(tail , i);
				}
			}

			tail = tail->next;
		}

	}
	return 0;
}
void* find_s_for_setmark(void* arg)
{
	find* ds = (find*) arg;
	tail = head;
	int count = tail->studentCount/2;

	if(ds->i == 1)
	{
		tail = head;
		pthread_mutex_lock(&mutex); //протокол входа в КС: закрыть
		while (tail != NULL)
		{
		
			for(int i = 0; i < count; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					setMarks(tail->students[i]);
				}
			}
			tail = tail->next;
		}	
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		tail = head;
		pthread_mutex_lock(&mutex);
		while (tail != NULL)
		{

			for(int i = count; i < tail->studentCount; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					setMarks(tail->students[i]);
				}
			}

			tail = tail->next;
		}
		pthread_mutex_unlock(&mutex);

	}
	return 0;
}

void* find_s_for_set_middle_mark(void* arg)
{
	find* ds = (find*) arg;
	tail = head;
	int count = tail->studentCount/2;

	if(ds->i == 1)
	{
		tail = head;
		pthread_mutex_lock(&mutex); //протокол входа в КС: закрыть
		while (tail != NULL)
		{
		
			for(int i = 0; i < count; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					float res = Get_average_score(tail->students[i]);
					printf("Средняя оценка студента %s = %f\n", tail->students[i].name, res);
				}
			}
			tail = tail->next;
		}	
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		tail = head;
		pthread_mutex_lock(&mutex);
		while (tail != NULL)
		{

			for(int i = count; i < tail->studentCount; i++)
			{
				if (strcmp (tail->students[i].name,ds->Sname)== 0)
				{
					float res = Get_average_score(tail->students[i]);
					printf("Средняя оценка студента %s = %f\n", tail->students[i].name, res);
				}
			}

			tail = tail->next;
		}
		pthread_mutex_unlock(&mutex);

	}
	return 0;

}