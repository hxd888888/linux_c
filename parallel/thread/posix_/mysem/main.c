#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include"mysem.h"

#define LEFT 	30000000
#define RIGHT 	30000200
#define THRNUM 	(RIGHT - LEFT + 1)
#define N		4

static mysem_t *sem;	

static void *thr_prime(void *p)
{
	int i,j,mark;
	mark = 1;
	i = (int)p;
	for(j = 2;j < i/2;j++)
	{
		if((i % j) == 0)
		{
			mark = 0;
			break;
		}	
		else
			mark = 1;
	}
	if(mark)
		printf("%d is a primer!\n",i);

	sleep(5);	

	mysem_add(sem,1);
	pthread_exit(NULL);
}

int main()
{
	int i;
	int err;
	pthread_t tid[THRNUM];

	sem = mysem_init(N);
	if(sem == NULL)
	{
		fprintf(stderr,"mysem_init() failed\n");
		exit(1);
	}	

	for(i = LEFT;i <= RIGHT; i++)
	{
		mysem_sub(sem,1);
		err = pthread_create(tid + (i - LEFT),NULL,thr_prime,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);	
		}
	}

	for(i = LEFT; i < RIGHT ; i++)
	{
		pthread_join(tid[i-LEFT],NULL);
	}
	
	mysem_destroy(sem);
	
	mysem_add(sem,1);
	exit(0);
}
