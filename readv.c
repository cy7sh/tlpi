#include <stdio.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (argc != 2 || (strcmp(argv[1], "--help") == 0)) {
		printf("Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);	
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("could not open file");
		exit(EXIT_FAILURE);
	}
	struct iovec iov[3];
	struct stat myStruct;
	int x;
	char str[100];
	iov[0].iov_base = &myStruct;
	iov[0].iov_len = sizeof(struct stat);
	iov[1].iov_base = &x;
	iov[1].iov_len = sizeof(x);
	iov[2].iov_base = &str;
	iov[2].iov_len = sizeof(str);
	ssize_t total = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	ssize_t numRead = readv(fd, iov, 3);
	if (numRead == -1) {
		perror("read error");
		exit(EXIT_FAILURE);
	}
	printf("Requested bytes: %ld; Read bytes: %ld\n", (long) total, (long) numRead);
	exit(EXIT_SUCCESS);
}
