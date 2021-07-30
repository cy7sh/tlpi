#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void fpathprint(const char *msg, int fd, int name)
{
	errno = 0;
	long lim = fpathconf(fd, name);
	if (lim != -1) {
		if (errno == 0)
			printf("%s: inderminate\n", msg);
		else {
			printf("invalid name\n");
			exit(EXIT_FAILURE);
		}
	}
}

int main()
{
	printf("stdin is %d\n", STDIN_FILENO);
	fpathprint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
	fpathprint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
	fpathprint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);
}
