#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int sigCnt[NSIG];
volatile sig_atomic_t gotSigint = 0;

void handler(int sig)
{
	if (sig == SIGINT)
		gotSigint = 1;
	else
		sigCnt[sig]++;
}

void printSigset(const char *prefix, const sigset_t *sigset)
{
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            printf("%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        printf("%s<empty signal set>\n", prefix);
}

int main(int argc, char *argv[])
{
	printf("%s: PID is %d\n", argv[0], getpid());
	struct sigaction action = {
		.sa_handler = handler,
		.sa_mask = 0,
		.sa_flags = 0
	};
	for (int i=1; i<NSIG; i++)
		sigaction(i, &action, NULL);
	if (argc>1)
	{
		sigset_t pendingMask, blockingMask, emptyMask;
		sigfillset(&blockingMask);
		sigprocmask(SIG_SETMASK, &blockingMask, NULL);
		int numSecs = atoi(argv[1]);
		printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
		sleep(numSecs);
		sigpending(&pendingMask);
		printf("%s: pending signals are:\n", argv[0]);
		printSigset("\t\t", &pendingMask);
		sigemptyset(&emptyMask);
		sigprocmask(SIG_SETMASK, &emptyMask, NULL);
	}
	while (!gotSigint)
		continue;
	for (int i=1; i<NSIG; i++) {
		if (sigCnt[i] != 0)
			printf("%s: signal %d caught %d times\n", argv[0], i, sigCnt[i]);
	}
}
