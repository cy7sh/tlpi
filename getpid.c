#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t mypid;
	mypid = getpid();
	printf("My pid is %ld\n", (long) mypid);
}
