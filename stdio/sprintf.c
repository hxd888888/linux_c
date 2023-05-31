#include<stdio.h>
#include<stdlib.h>

#define BUFSIZE 1024

int main()
{
	char buf[BUFSIZE];
	int year = 2023;
	int month = 4;
	int day = 18;
	sprintf(buf,"%d-%d-%d",year, month, day);
	puts(buf);
	exit(0);
	
}
