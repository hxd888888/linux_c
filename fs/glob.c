#include<stdio.h>
#include<stdlib.h>
#include<glob.h>

//#define PAT "/etc/a*.conf"

#define PAT "/etc/*"

#if 0
static int errfunc_(const char *errpath,int errno)
{
	puts(errpath);
	fprintf(stderr,"ERROR MSG:5s\n",strerror(error));
}
#endif
int main()
{
	glob_t globres;
	int i, err;
	err = glob(PAT,0,NULL,&globres);
	if(err)
	{
		printf("Error code = %d\n",err);
		exit(1);

	}
	
	for(i = 0;i< globres.gl_pathc;i++)
	{
		puts(globres.gl_pathv[i]);
	}
	globfree(&globres);

	exit(0);
}
