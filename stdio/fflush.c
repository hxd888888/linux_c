#include<stdio.h>
#include<stdlib.h>

int main()
{
	printf("before while()!");
//	fflush(stdout);
	fflush(NULL);
	while(1);
	printf("after whiel()!");
	exit(0);
}	
