#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

#define LEFT 	30000000
#define RIGHT 	30000200
#define THRNUM 	(RIGHT - LEFT)

struct thr_arg_st
{
	int n;
};

static void *thr_prime(void *p)
{
	int i,j,mark;
	mark = 1;
	i = ((struct thr_arg_st *)p)->n;
//	free(p);

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
	pthread_exit(p);
}

int main()
{
	int i;
	int err;
	pthread_t tid[THRNUM];
	struct thr_arg_st *p;
	void * ptr;
	
	for(i = LEFT;i < RIGHT; i++)
	{
		p = malloc(sizeof(*p));		
		if(p == NULL)
		{
			perror("malloc()");
			exit(1);
		}
		p->n = i;

		err = pthread_create(tid + (i - LEFT),NULL,thr_prime,p);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);	
		}
	}

	for(i = LEFT; i < RIGHT ; i++)
	{
		pthread_join(tid[i-LEFT],&ptr);
		free(ptr);
	}

	exit(0);
}
