#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	if (argc < 3 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: <source file> <destination file>\n");
		exit(EXIT_FAILURE);
	}
	int sourceFd = open(argv[1], O_RDONLY);
	int destFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	char *buf = malloc(BUF_SIZE);
	int numRead, currentOffset, endOffset;
	endOffset = lseek(sourceFd, 0, SEEK_END);
	if (endOffset == -1 || lseek(sourceFd, 0, SEEK_SET) == -1) {
		perror("seek error");
		exit(EXIT_FAILURE);
	}
	while (1) {
		currentOffset = lseek(sourceFd, 0, SEEK_CUR);
		if ((numRead = read(sourceFd, buf, BUF_SIZE)) > 0) {
			if (write(destFd, buf, numRead) != numRead) {
				printf("write error\n");
				exit(EXIT_FAILURE);
			}
	 	}
		if (currentOffset == endOffset) {
			break;
		}
	}
	if ((close(sourceFd) == -1) || (close(destFd) == -1)) {
		perror("error closing file");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
