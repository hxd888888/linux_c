#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	pid_t pid;
	printf("[%d]:Begin!\n",getpid());
	fflush(NULL);	//对比有和没有刷新流
	pid = fork();
	if(pid < 0)	
	{
		perror("fork()");
		exit(1);
	}
	else if(pid > 0)
	{
	//	sleep(1);
		printf("[%d]:This is parent process!\n",getpid());
	}
	else if(pid == 0)
	{
		printf("[%d]:This is child process!\n",getpid());
	}
	printf("[%d]:End!\n",getpid());
	exit(0);
}
