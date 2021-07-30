#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
	clearenv();
	for (int i=1; i<argc; i++) {
		if (putenv(argv[i]) != 0) {
			perror("error setting environment");
			exit(EXIT_FAILURE);
		}
	}
	if (setenv("GREET", "Hello world", 0) == -1) {
		perror("error setting GREET");
		exit(EXIT_FAILURE);
	}
	unsetenv("BYE");
	for (char **ep = environ; *ep != NULL; ep++)
		puts(*ep);
	exit(EXIT_SUCCESS);
}
