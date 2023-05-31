#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
/*
 *   ymdHMS 
 *	 y: 2,4
 *	 H:12,24
 * 	 
*/
#define BUFSIZE 1024
#define FMTSTRSIZE 1024

int main(int argc,char **argv)
{
	FILE *fp = stdout;
	time_t stamp;
	struct tm *tm;	
	char timestr[BUFSIZE];	
	int c;
	char fmtstr[FMTSTRSIZE];
	fmtstr[0] = '\0';
	stamp = time(NULL);
	tm = localtime(&stamp);
	while(1)
	{
		c = getopt(argc,argv,"-H:MSy:md");
		if(c < 0)
			break;
		switch(c)
		{
			case 1:
				if(fp == stdout)		//先入为主（全部都写入、后入为主）
				{
					fp = fopen(argv[optind -1],"w");
					if(fp == NULL)
					{
						perror("fopen()");
						fp = stdout;
					}
				}
				break;
			case 'H':
				if(strcmp(optarg,"12") == 0)
					strncat(fmtstr,"%I(%P) ",FMTSTRSIZE);
				else if(strcmp(optarg,"24") == 0)
					strncat(fmtstr,"%H ",FMTSTRSIZE);
				else
					fprintf(stderr,"Ivalid arguement");
				break;
			case 'M':
				strncat(fmtstr,"%M ",FMTSTRSIZE);
				break;
			case 'S':
				strncat(fmtstr,"%S ",FMTSTRSIZE);
				break;
			case 'y':
				if(strcmp(optarg,"4") == 0)
					strncat(fmtstr,"%Y ",FMTSTRSIZE);
				else if(strcmp(optarg,"2") == 0)
					strncat(fmtstr,"%y ",FMTSTRSIZE);
				else 
					fprintf(stderr,"Ivalid arguement");
				break;
			case 'm':
				strncat(fmtstr,"%m ",FMTSTRSIZE);
				break;
			case 'd':
				strncat(fmtstr,"%d ",FMTSTRSIZE);
				break;
			default:
				break;
		}
	}
	strncat(fmtstr,"\n",FMTSTRSIZE);
	strftime(timestr,BUFSIZE,fmtstr,tm);
	fputs(timestr,fp);	
	if(fp != stdout)
		fclose(fp);
	exit(0);
}
