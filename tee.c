#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *buf = malloc(1024);
	int outputFd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	ssize_t numRead;
	while ((numRead = read(STDIN_FILENO, buf, 1024)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			perror("could not write to the file");
			exit(EXIT_FAILURE);;
		}
		if (write(STDOUT_FILENO, buf, numRead) != numRead) {
			perror("could not write to STDOUT");
		}
	}
	if (close(outputFd == -1)) {
		perror("could not close the file");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
