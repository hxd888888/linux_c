#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	printf("Begin!\n");
	fflush(NULL);					//注意刷新流
	execl("/bin/date","date","+%s",NULL);
	perror("exec()");
	exit(1);
	

	printf("End!\n");
	
	
	exit(0);
}
