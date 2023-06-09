#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<sys/time.h>

#define CPS 	10
#define BUFSIZE 10
#define BURST 	100

static volatile int token = 1;

static void alrm_handler(int s)
{
	token ++;
	if(token > BURST)
		token = BURST;
}

int main(int argc,char **argv)
{
	int sfd,dfd = 1;
	char buf[BUFSIZE];
	int len,ret;
	int pos;
	struct itimerval itv;

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	signal(SIGALRM,alrm_handler);
//	alarm(1);
	
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;	

	if(setitimer(ITIMER_REAL,&itv,NULL) < 0)
	{
		perror("setitimer()");
		exit(1);
	}

	do																//注释均为避免信号打断阻塞的系统调用而添加
	{															   //
		sfd = open(argv[1],O_RDONLY);
		if(sfd < 0)
		{
			if(errno != EINTR)
			{	
				perror("open()");
				exit(1);
			}
		
		}
	}while(sfd < 0);										//

	while(1)
	{
		while(token <= 0)
			pause();		//pause()避免忙等，cpu将不会占满
		token --;

		while((len = read(sfd,buf,BUFSIZE)) < 0)
		{
			if(len < 0)
			{
				if(errno == EINTR)					 //
					continue;						//
				perror("read()");			
				break;
			}
		}
		if(len == 0)
			break;
		pos = 0;
		while(len > 0)
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{
				if(errno == EINTR)			//
					continue;			   //
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}
	close(sfd);
	exit(0);
}
