#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>

int main()
{
	int fd = open("test", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	struct iovec iov[3];
	struct stat myStruct;
	int x = 9999;
	char str[100] = "abcdefghijklmnopqrstuzwxyz";
	iov[0].iov_base = &myStruct;
	iov[0].iov_len = sizeof(struct stat);
	iov[1].iov_base = &x;
	iov[1].iov_len = sizeof(x);
	iov[2].iov_base = &str;
	iov[2].iov_len = sizeof(str);
	ssize_t total = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	ssize_t numWritten = writev(fd, iov, 3);	
	printf("wrote %ld bytes\n", numWritten);
}
