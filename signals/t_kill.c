#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	if (argc != 3 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		printf("Usage: %s <sig-num> <pid>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	int sig = atoi(argv[1]);
	int s = kill(atol(argv[2]), sig);
	if (sig != 0) {
		if (s == -1) {
			perror("kill error");
			exit(EXIT_FAILURE);
		}
	} else { // Null signal: process existence check
		if (s == 0) {
			printf("Process exists and we can send it a signal\n");
		} else {
			if (errno == EPERM)
				printf("Process exists, but we don't have permission to send it a signal\n");
			else if (errno == ESRCH)
				printf("Process does not exist\n");
			else {
				printf("Kill error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
