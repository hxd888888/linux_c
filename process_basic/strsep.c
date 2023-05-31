#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DELIMS " \t\n"

int main()
{
	char *tok;
	char *linebuf = NULL;
	size_t linebuf_size = 0;
	getline(&linebuf,&linebuf_size,stdin);
	printf("%s\n",linebuf);
	while(1)
	{
		tok = strsep(&linebuf,DELIMS);
		if(tok == NULL)
			break;
		printf("%s\n",tok);
	}
}
