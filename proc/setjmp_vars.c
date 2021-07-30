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
	int nvar; // this might be in RAM or register (depends on compiler optimization)
	register int rvar; // this will be in register
	volatile int vvar; // this will be in RAM

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
