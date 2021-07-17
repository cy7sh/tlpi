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
	int outputFd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	ssize_t numRead;
	while ((numRead = read(1, buf, 1024)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			perror("could not write the whole buffer");
			exit(EXIT_FAILURE);;
		}
	}
	if (close(outputFd == -1)) {
		perror("could not close the file");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
