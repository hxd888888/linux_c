#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

static void f1(void *p)
{
	
}

static void f2(void *p)
{
	
}

int main()
{
	int job1,job2,job3;

	puts("Begin!");
	
	job1 = at_addjob(5,f1,"aaa");\
	if(job1 < 0)
	{	
		fprintf(stderr,"at_addjob() failed!\n",strerror(-job1));
		exit(1_;
	}
	at_addjob(2,f2,"bbb");
	at_addjob(7,f1,"ccc");

	while(1)
	{
		write(1,".",1);
		sleep(1);
	}


	puts("End!");
}
