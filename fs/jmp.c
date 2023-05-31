#include<stdio.h>
#include<stdlib.h>
#include<setjmp.h>
static jmp_buf save;

static void d()
{
	printf("%s():Begin.\n",__FUNCTION__);
	printf("%s():Jump now!\n",__FUNCTION__);
	longjmp(save,6);
 	printf("%s():End.\n",__FUNCTION__);
}

static void c()
{
	printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():Call d().\n",__FUNCTION__);
    d();
    printf("%s():d() Return.\n",__FUNCTION__);
    printf("%s():End.\n",__FUNCTION__);
}

static void b()
{
	printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():Call c().\n",__FUNCTION__);
    c();
    printf("%s():c() Return.\n",__FUNCTION__);
    printf("%s():End.\n",__FUNCTION__);
}

static void a()
{
	int ret;
	printf("%s():Begin.\n",__FUNCTION__);
	ret = setjmp(save);
	if(ret == 0)
	{
    	printf("%s():Call b().\n",__FUNCTION__);
    	b();
    	printf("%s():b() Return.\n",__FUNCTION__);
	}
	else
	{
   		 printf("%s():Jumped back here with code %d\n",__FUNCTION__,ret);
	}
}

int main()
{
	printf("%s():Begin.\n",__FUNCTION__);
	printf("%s():Call a().\n",__FUNCTION__);
	a();
	printf("%s():a() Return.\n",__FUNCTION__);
	printf("%s():End.\n",__FUNCTION__);


	exit(0);
}
