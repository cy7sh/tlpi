#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	printf("O_SYNC is %x\n", O_SYNC);
	int fd = open("note", O_RDWR | O_SYNC | O_CREAT);
	int flags = fcntl(fd, F_GETFL);
	printf("Flags: %x\n", flags);
	printf("And'd: %x\n", flags & O_SYNC);
	printf("Append And'd: %x\n", flags & O_APPEND);
	printf("And'd: %x\n", flags & O_CREAT);
	int accessMode = flags & O_ACCMODE;
	printf("Mode: %x\n", O_ACCMODE);
	printf("Access: %x\n", accessMode);
	flags |= O_APPEND;
	fcntl(fd,  F_SETFL, flags);
	flags = fcntl(fd, F_GETFL);
	printf("Append And'd: %x\n", flags & O_APPEND);
	close(fd);
}
