#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

static void int_handler(int s)
{
	write(1,"!",1);
}

int main()
{
	int i,j;
	sigset_t set,oldset,saveset;

	signal(SIGINT,int_handler);
	sigemptyset(&set);
	sigaddset(&set,SIGINT);	
	sigprocmask(SIG_UNBLOCK,&set,&saveset);
	sigprocmask(SIG_BLOCK,&set,&oldset);
	for(j = 0;j < 1000;j++)
	{
//		sigprocmask(SIG_BLOCK,&set,NULL);
		for(i = 0;i < 5;i++)
		{
			write(1,"*",1);
			sleep(1);
		}
		write(1,"\n",1);
//		sigprocmask(SIG_UNBLOCK,&set,NULL);
/*用sigsuspend()的原子化操作来代替
		sigset_t tmpset;
		sigprocmask(SIG_SETMASK,&oldset,&tmpset);
		pause();
		sigprocmask(SIG_SETMASK,&tmpset,NULL);
*/
		sigsuspend(&oldset);
	}
	sigprocmask(SIG_SETMASK,&saveset,NULL);
	exit(0);
}
