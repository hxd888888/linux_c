#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
//	alarm(5);

//	alarm(10);
//	alarm(1);
//	alarm(5);

//	alarm(10);
//	alarm(5);
//	alarm(1);

	alarm(10);
	alarm(1);
	alarm(5);

	
	while(1);
//		pause();			不是忙等，cpu未占满，若cpu占满
	exit(0);
}
