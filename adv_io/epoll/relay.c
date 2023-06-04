#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/select.h>
#include<sys/epoll.h>

#define 	TTY1	"/dev/tty10"
#define 	TTY2	"/dev/tty11"
#define 	BUFSIZE 1024

enum
{
	STATE_R = 1,
	STATE_W,
	STATE_AUTO,
	STATE_Ex,
	STATE_T	
};


struct fsm_st
{
	int state;
	int sfd;
	int dfd;
	char buf[BUFSIZE];
	int len;
	int pos;
	char *errstr;
};

static void fsm_driver(struct fsm_st *fsm)
{
	int ret;
	
	switch(fsm->state)
	{
		case STATE_R:
			fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
			if(fsm->len == 0)
				fsm->state == STATE_T;
			else if(fsm->len < 0)
				{
					if(errno == EAGAIN)		//为假错
						fsm->state = STATE_R;		//假错，再读
					else 
					{
						fsm->errstr = "read()";
						fsm->state = STATE_Ex;		//真错，异常中止态
							
					}
				}
				else
				{
					fsm->pos = 0;
					fsm->state = STATE_W;
				}
			break;
		case STATE_W:
			ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
			if(ret < 0)
			{
				if(errno == EAGAIN)		//为假错
					fsm->state = STATE_W;
				else
				{
					fsm->errstr = "write()";
					fsm->state = STATE_Ex;
				}
			}
			else
			{
				fsm->pos += ret;
				fsm->len -= ret;
				if(fsm->len == 0)
					fsm->state  = STATE_R;
				else 
					fsm->state = STATE_W;
			}
			break;
		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			/*do sth*/
			break;
		default:
			abort();
			break;
	}
}

static int max(int fd1,int fd2)
{
	if(fd1 > fd2)
		return fd1;
	else
		return fd2;
}

static void relay(int fd1,int fd2)
{
	int fd1_save,fd2_save;
	struct fsm_st fsm12,fsm21;
	struct epoll_event ev;
	int epfd;

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);

	fd2_save = fcntl(fd2,F_SETFL);	
	fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);
	
	fsm12.state = STATE_R;
	fsm12.sfd = fd1;
	fsm12.dfd =fd2;
	
	fsm21.state = STATE_R;
	fsm21.sfd = fd2;
	fsm21.dfd = fd1;

	epfd = epoll_create(10);
	if(epfd < 0)
	{
		perror("epoll_create()");
		exit(1);
	}	
	
	ev.events = 0;
	ev.data.fd = fd1;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);


	ev.events = 0;
	ev.data.fd = fd2;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);

	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		//布置监视任务
			 
		ev.data.fd = fd1;
		ev.events = 0;
		if(fsm12.state == STATE_R)
			ev.events |= EPOLLIN;
		if(fsm21.state == STATE_W)
			ev.events |= EPOLLOUT;
		epoll_ctl(epfd,EPOLL_CTL_MOD,fd1,&ev);
	

		ev.data.fd = fd2;
		ev.events = 0;
		if(fsm12.state == STATE_W)
			ev.events |= EPOLLOUT;
		if(fsm21.state == STATE_R)
			ev.events |= EPOLLIN;
		epoll_ctl(epfd,EPOLL_CTL_MOD,fd2,&ev);
		

		//监视
		if(fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
		{
			while(epoll_wait(epfd,&ev,1,-1) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("epoll_wait()");
				exit(1);
			}
		}
	
		//查看监视结果
		if( ev.data.fd == fd1 && ev.events & EPOLLIN \
			 || ev.data.fd == fd2 && ev.events & EPOLLOUT \
			 || fsm12.state > STATE_AUTO)
			fsm_driver(&fsm12);
		if(ev.data.fd == fd2 && ev.events & EPOLLIN \
			 || ev.data.fd == fd1 && ev.events & EPOLLOUT \
			 || fsm21.state > STATE_AUTO)
			fsm_driver(&fsm21);
	}
	
	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);
	
	close(epfd);
	
}

int main()
{
	int fd1,fd2;

	fd1 = open (TTY1,O_RDWR);
	if(fd1 < 0)
	{
		perror("open()");
		exit(1);
	}
	write(fd1,"TTY1\n",5);

	fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
	if(fd2 < 0)
	{
		perror("open()");
		exit(1);
	}
	write(fd2,"TTY2\n",5);

	relay(fd1,fd2);
	
	close(fd2);
	close(fd1);

	exit(0);
}
