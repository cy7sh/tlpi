#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int flag = getopt(argc, argv, "+a");
	/* When there's no flag flag's bits are set to high. We want to prevent flags after filename*/
	if (argc < 2 || flag == '?' || (flag == 0xffffffff && argc > 2)) {
		printf("Usage: %s [-a] <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *buf = malloc(1024);
	int outputFd;
	if (flag == 'a') {
		outputFd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	} else {
		outputFd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	}
	ssize_t numRead;
	while ((numRead = read(STDIN_FILENO, buf, 1024)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			perror("could not write to the file");
			exit(EXIT_FAILURE);
		}
		if (write(STDOUT_FILENO, buf, numRead) != numRead) {
			perror("could not write to STDOUT");
			exit(EXIT_FAILURE);
		}
	}
	if (close(outputFd == -1)) {
		perror("could not close the file");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
