#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

int main()
{
	int i;
	signal(SIGINT,SIG_IGN);
	for(i = 0;i < 10;i++)
	{
		write(1,"*",1);
		sleep(1);
	}

	exit(0);
}
