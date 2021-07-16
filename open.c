#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd;
	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1) {
		perror("error opening file");
		return 1;
	}
	char buf[1024];
	int numread;
	numread = read(fd, &buf, 1024);
	printf(buf);
	printf("\n");
	printf("read: %d \n", numread);
	if (close(fd) == -1) {
		perror("error closing file");
		return 1;
	}
}
