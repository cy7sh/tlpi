#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <fchdir | chdir>\n", argv[0]);
		puts("Use the time command to time operation");
		exit(EXIT_FAILURE);
	}
	if (!strcmp(argv[1], "fchdir")) {
		puts("starting test");
		int afd = open("test", O_RDONLY);
		int bfd = open("test1", O_RDONLY);
		for (int i=0; i<1000000; i++) {
			fchdir(afd);
			fchdir(bfd);
		}
		puts("finished");
	} else if (!strcmp(argv[1], "chdir")) {
		puts("starting test");
		for (int i=0; i<1000000; i++) {
			chdir("test");
			chdir("test1");
		}
		puts("finished");
	} else {
		puts("Invalid option. Use --help");
	}
}
