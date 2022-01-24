#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Print lis of signals within a signal set */
void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
	int cnt = 0;
	for (int sig = 1; sig < NSIG; sig++) {
		if (sigismember(sigset, sig)){
			cnt++;
			fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
		}
	}
	if (cnt == 0) {
		fprintf(of, "%s<empty signal set>\n", prefix);
	}
}

/* Print list of blocked signals for this process */
int printSigMask(FILE *of, const char *msg) {
	if (msg != NULL)
		fprintf(of, "%s", msg);
	sigset_t currentMask;
	if (sigprocmask(SIG_BLOCK, NULL, &currentMask) == -1)
		return -1;
	printSigset(of, "\t\t", &currentMask);
	return 0;
}

/* Print signals pending for this process */
int printPendingSigs(FILE *of, const char *msg) {
	if (msg != NULL)
		fprintf(of, "%s", msg);
	sigset_t pendingSigs;
	if (sigpending(&pendingSigs) == -1)
		return -1;
	printSigset(of, "\t\t", &pendingSigs);
	return 0;
}

int main() {
	sigset_t maskSet;
	sigemptyset(&maskSet);
	sigaddset(&maskSet, SIGINT);
	printf("blocking following signals:\n");
	printSigset(stdout, "\t\t", &maskSet);
	if (sigprocmask(SIG_BLOCK, &maskSet, NULL) == -1) {
		perror("sigprocmask error");
		exit(EXIT_FAILURE);
	}
	printSigMask(stdout, "currently blocked signals:\n");
	printf("sleeping for 5 seconds\n");
	sleep(5);
	printPendingSigs(stdout, "pending signals:\n");
	exit(EXIT_SUCCESS);
}
