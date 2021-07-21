#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

void f2()
{
	longjmp(env, 2);
}

void f1()
{
	longjmp(env, 1);
	f2();
}


int main(int argc, char *argv[])
{
	switch (setjmp(env)) {
	case 0:
		printf("initial setjmp. calling f1\n");
		f1();
	case 1:
		printf("did a long jump from f1\n");
	case 2:
		printf("did a long jump from f2\n");
	}
}
