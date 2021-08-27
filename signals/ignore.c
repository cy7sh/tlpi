#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	/* set disposition to SIG_IGN */
	printf("ignoring signal SIGINT\n");
	struct sigaction action = {
		.sa_handler = SIG_IGN,
		.sa_flags = 0
	};
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	/* block signal */
//	sigset_t blockMask, oldMask;
//	sigemptyset(&blockMask);
//	sigaddset(&blockMask, SIGINT);
//	if (sigprocmask(SIG_BLOCK, &blockMask, &oldMask) == -1) {
//		perror("block error");
//		return 1;
//	}
//	printf("blocking signal SIGINT\n");
	printf("pausing\n");
	pause();
}
