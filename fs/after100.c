#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#define BUFSIZE 1024

int main()
{
	time_t stamp;
	struct tm *tm;	
	char timestr[BUFSIZE];	
	stamp = time(NULL);
	tm = localtime(&stamp);
	strftime(timestr,BUFSIZE,"Now:%Y-%m-%d",tm);
	puts(timestr);
	tm->tm_mday += 100;
	(void)mktime(tm);
	strftime(timestr,BUFSIZE,"After 100 days:%Y-%m-%d",tm);
	puts(timestr);
		
	
	exit(0);
}
