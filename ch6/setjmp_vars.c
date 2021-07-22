#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void doJump(int nvar, int rvar, int vvar)
{
	printf("Jumping: nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
	longjmp(env, 1);
}

int main(int argc, char *argv[])
{
	int nvar;
	register int rvar;
	volatile int vvar;

	nvar = 111;
	rvar = 222;
	vvar = 333;

	if (setjmp(env) == 0) {
		nvar = 777;
		rvar = 888;
		vvar = 999;
		/* value of rvar should reset to 222 after jump as it is stored in register */
		doJump(nvar, rvar, vvar);
	} else {
		printf("After jump: nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
	}
}
