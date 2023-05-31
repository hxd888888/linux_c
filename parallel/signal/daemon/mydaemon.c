#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<errno.h>
#include<string.h>
#include<signal.h>

#define FNAME "/tmp/out"

static FILE *fp;

static int daemonsize(void)
{
	pid_t pid;
	int fd;
	pid = fork();
	if(pid < 0)
		return -1;
	if(pid > 0)
		exit(0);
	fd = open("/dev/null",O_RDWR);
	if(fd < 0)
		return -1;
	dup2(fd,0);
	dup2(fd,1);
	dup2(fd,2);
	if(fd > 2)
		close(fd);

	setsid();
	chdir("/");
	return 0;
}

static void daemon_exit(int s)
{
/*
	if(s == SIGINT)
		;
	if(s == SIGQUIT)
		;
	if(s == SIGTERM)
		;
*/
	fclose(fp);
	closelog();
}

int main()
{
	int i;
	struct sigaction sa;

/*

	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sa.sa_flags = 0;
	sigaction(SIGINT,&sa,NULL);

	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGINT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sa.sa_flags = 0;
	sigaction(SIGQUIT,&sa,NULL);


	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGTERM,&sa,NULL);
*/
	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sigaddset(&sa.sa_mask,SIGINT);
	sa.sa_flags = 0;
	
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
	sigaction(SIGTERM,&sa,NULL);
/*	
	signal(SIGINT,daemon_exit);			//如果多个信号到来有可重入的风险，因此换为sigaction()
	signal(SIGQUIT,daemon_exit);
	signal(SIGTERM,daemon_exit);
*/

	openlog("mydaemon",LOG_PID,LOG_DAEMON);

	if(daemonsize())
	{
		syslog(LOG_ERR,"daemonsize() dailed!");
		exit(1);
	}
	else
	{
		syslog(LOG_INFO,"daemonsize() succeed!");
	}

	fp = fopen(FNAME,"w");
	if(fp == NULL)
	{
		syslog(LOG_ERR,"fopen():%s",strerror(errno));
		exit(1);
	}
	syslog(LOG_INFO,"%s is opened.",FNAME);
	for(i = 0;;i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		syslog(LOG_DEBUG,"%d is printed.",i);
		sleep(1);
	}


	exit(0);
}
