#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<glob.h>
#include<string.h>
#include<wait.h>
#define DELIMS  " \t\n"

struct cmd_st
{
	glob_t globres;
};

static void prompt(void)
{
	printf("mysh-0.1$ ");
}

static void  parse(char *line,struct cmd_st *res)
{
	char *tok = NULL;
	int i = 0;
	while(1)
	{
		tok = strsep(&line,DELIMS);
		if(tok == NULL)
			break;
		if(tok[0] == '\0')
			continue;
//		printf("%s\n",tok);
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
		i = 1;
	}
//	for(int j = 0;j < res->globres.gl_pathc;j++)
//		printf("%s\n",res->globres.gl_pathv[j]);
}

int main()
{
	pid_t pid;
	char *linebuf = NULL;
	size_t linebuf_size = 0;
	struct cmd_st cmd;
	while(1)
	{
		prompt();


		if(getline(&linebuf,&linebuf_size,stdin) < 0 )
			break;
//		printf("%s\n",linebuf);
		parse(linebuf,&cmd);

		if(0)				//内部命令
		{
		}	
		else				//外部命令
		{
			pid = fork();
			if(pid < 0 )
			{
				perror("fork()");
				exit(1);
			}
			if(pid  == 0)
			{
//				for(int i = 0;cmd.globres.gl_pathv[i] != NULL;i++)
//				{
//					printf("%s\n",cmd.globres.gl_pathv[i]);
//				}
				execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}
			else
			{
				wait(NULL);
			}
		}
		
	}
	exit(0);
}
