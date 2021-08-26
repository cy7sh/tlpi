#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigHandler(int sig)
{
	printf("Ouch!\n");
}

int main()
{
	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		perror("signal disposition");
		return 1;
	}
	for (int i=0; ; i++) {
		printf("%d\n", i);
		sleep(3);
	}
}
