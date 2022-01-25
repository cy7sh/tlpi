#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void handler(int sig) {
	printf("caught a signal\n");
}

int main() {
	struct sigaction action = {
		.sa_handler = handler,
		.sa_flags = SA_RESETHAND
	};
	sigemptyset(&action.sa_mask);
	printf("handling SIGINT\n");
	if (sigaction(SIGINT, &action, NULL) == -1) {
		perror("sigaction error");
		exit(EXIT_FAILURE);
	}
	for (;;) {
		pause();
	}
}
