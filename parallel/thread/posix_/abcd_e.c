#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define THRNUM 			4	

static pthread_mutex_t mut[THRNUM];

static int next(int n)
{
	if(n + 1 == THRNUM)
		return 0;
	else
		return (n+1);
}

static void *thr_abcd(void *p)
{
	int n = (int)p;
	int c = 'a' + (int)p;

	while(1)
	{	
		pthread_mutex_lock(mut+n);
		write(1,&c,1);
		pthread_mutex_unlock(mut+next(n));
	}
	
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[THRNUM];
	int i,err;

	for(i = 0; i < THRNUM;i++)
	{
		pthread_mutex_init(mut+i,NULL);
		pthread_mutex_lock(mut+i);

		err = pthread_create(tid + i,NULL,thr_abcd,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_creat():%s\n",strerror(err));
			exit(1);
		}
	}
	
	pthread_mutex_unlock(mut+0);
	
	alarm(5);

	for(i = 0;i < THRNUM;i++)
	{
		pthread_join(tid[i],NULL);
	}	
	
	exit(0);	
}
