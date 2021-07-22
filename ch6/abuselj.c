#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

int f1()
{
	setjmp(env);	
	printf("f1 ran\n");
	return 0;
}

void f2()
{
	longjmp(env, 1);
	printf("f2 ran\n");
}

void f3()
{
	longjmp(env, 2);
	printf("f3 ran\n");
}

int main()
{
	f1();
	f2();
	f3();
}
