#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3 || (strcmp(argv[1], "--help") == 0)) {
		printf("Usage: %s <filename> <offset>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int fd = open(argv[1], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1) {
		perror("open error");
		exit(EXIT_FAILURE);
	}
	off_t off = atoll(argv[2]);
	if (lseek(fd, off, SEEK_SET) == -1) {
		perror("seek error");
		exit(EXIT_FAILURE);
	}
	if (write(fd, "test", 4) == -1) {
		perror("write error");
		exit(EXIT_FAILURE);
	}
}
