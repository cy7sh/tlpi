#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <signal.h>

char *str2;
int handled = 0;

void handler(int sig)
{
	crypt(str2, "xx");
	handled++;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s str1 str2\n", argv[0]);
		return 1;
	}
	str2 = argv[2];
	char *cr1 = strdup(crypt(argv[1], "xx"));
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction error");
		return 1;
	}
	int mismatch = 0;
	for (int i=1; ; i++) {
		if (strcmp(crypt(argv[1], "xx"), cr1)) {
			mismatch++;
			printf("mismatch on call %d (mismatch=%d handled=%d)\n", i, mismatch, handled);
		}
	}
}
