#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#define CPS 	10
#define BUFSIZE 10

static volatile int loop = 1;

static void alrm_handler(int s)
{
	alarm(1);
	loop = 1;
}

int main(int argc,char **argv)
{
	int sfd,dfd = 1;
	char buf[BUFSIZE];
	int len,ret;
	int pos;
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	alarm(1);
	signal(SIGALRM,alrm_handler);

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
		while(!loop)
			pause();		//pause()避免忙等，cpu将不会占满
		loop = 0;

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
