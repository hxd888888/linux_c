#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main()
{
	FILE *fp;
	fp = fopen("temp","r");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}
	puts("OK");
	exit(0);
}
