#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define THRNUM 			4	



static void *thr_abcd(void *p)
{
	int c = 'a' + (int)p;

	while(1)
		write(1,&c,1);
	
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[THRNUM];
	int i,err;

	for(i = 0; i < THRNUM;i++)
	{
		err = pthread_create(tid + i,NULL,thr_abcd,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_creat():%s\n",strerror(err));
			exit(1);
		}
	}
	
	alarm(5);

	for(i = 0;i < THRNUM;i++)
	{
		pthread_join(tid[i],NULL);
	}	
	
	exit(0);	
}
