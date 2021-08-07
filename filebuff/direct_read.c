#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
	if (argc<3 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s file length [offset [alignment]]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	size_t length = strtol(argv[2], NULL, 10);
	off_t offset = (argc > 3) ? strtol(argv[3], NULL, 10) : 0;
	size_t alignment = (argc > 4) ? strtol(argv[4], NULL, 10) : 4096;
	int fd = open(argv[1], O_RDONLY | O_DIRECT);
	if (fd == -1) {
		printf("could not open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	char *buf = (char *) memalign(alignment * 2, length + alignment) + alignment;
	lseek(fd, offset, SEEK_SET);
	size_t numRead = read(fd, buf, length);
	if (numRead == -1) {
		perror("read error");
		exit(EXIT_FAILURE);
	}
	printf("Read %ld bytes\n", numRead);
}
