#include <stdio.h>
#include <sys/types.h>

int main()
{
	pid_t mypid;
	mypid = getpid();
	printf("My pid is %ld\n", (long) mypid);
}
