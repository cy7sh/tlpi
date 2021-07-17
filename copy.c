#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: %s old-file new-file\n", argv[0]);
		exit(1);
	}
	int inputFd;
	printf("opening file %s\n", argv[1]);
	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		printf("error opening file %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	int openFlags;
	int filePerms;
	int outputFd;
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IRGRP | S_IROTH; /* rw-rw-r*/
	printf("opening file %s\n", argv[2]);
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		printf("error opening file %s: %s\n", argv[2], strerror(errno));
		exit(1);
	}
	/* transfer data untl EOF or error encounter */
	ssize_t numRead;
	char *buf = malloc(1024);
	printf("copying data...\n");
	while ((numRead = read(inputFd, buf, 1024)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			perror("couldn't write whole buffer");
			exit(1);
		}
	}
	if (numRead == -1) {
		perror("read error");
		exit(1);
	}
	if (close(inputFd) == -1) {
		perror("error closing input file");
		exit(1);
	}
	if (close(outputFd) == -1) {
		perror("error closing output file");
		exit(1);
	}
	exit(0);
}
