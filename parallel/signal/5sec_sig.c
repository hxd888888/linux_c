#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
	
static volatile int loop = 1;		//volatile  去到内存的真正空间去取值  如果没有优化程序时将会出现死循环

static void alrm_handler(int s)
{
	loop = 0;
}

int main()
{
	int64_t count;
	alarm(5);
	signal(SIGALRM,alrm_handler);
	while(loop)
		count++;
	printf("%ld\n",count);	
	exit(0);
	
}	
