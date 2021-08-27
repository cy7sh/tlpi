#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	if (argc<4 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		printf("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);
		return 1;
	}
	pid_t pid = atoi(argv[1]);
	int numSigs = atoi(argv[2]);
	int sig = atoi(argv[3]);
	printf("%s: sending %d signal to process %d %d times\n", argv[0], sig, pid, numSigs);
	for (int i=0; i<numSigs; i++) {
		if (kill(pid, sig) == -1) {
			perror("kill error");
			return 1;
		}
	}
	if (argc>4) {
		if (kill(pid, atoi(argv[4])) == -1) {
			perror("kill error");
			return 1;
		}
	}
	printf("%s: exiting\n", argv[0]);
}
