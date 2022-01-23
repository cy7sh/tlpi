#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigHandler(int sig) {
	static int count = 0;
	if (sig == SIGINT) {
		count++;
		printf("Caught SIGINT (%d)\n", count);
		return;
	}
	printf("Caught SIGQUIT");
	exit(0);
}

int main() {
	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		printf("signal error\n");
		exit(1);
	}
	if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
		printf("signal error\n");
		exit(1);
	}
	for (;;) {
		pause();
	}
}
