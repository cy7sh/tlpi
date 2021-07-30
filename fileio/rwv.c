/* Exercise 5-7 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct iovec {
	void *iov_base;
	size_t iov_len;
};

ssize_t writev(int fd, struct iovec *iov, int iovcnt)
{
	ssize_t totalWritten = 0;
	for (int i=0; i<iovcnt; i++) {
		ssize_t written = write(fd, &iov[i].iov_base, iov[i].iov_len);
		if (written == -1)
			return -1;
		totalWritten += written;
	}
	return totalWritten;
}

ssize_t readv(int fd, struct iovec *iov, int iovcnt)
{
	ssize_t totalRead = 0;
	for (int i=0; i<iovcnt; i++) {
		ssize_t numRead = read(fd, &iov[i].iov_base, iov[i].iov_len);
		if (numRead == -1)
			return -1;
		totalRead += numRead;
	}
	return totalRead;
}

void first()
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

void second()
{
	int fd = open("test", O_RDONLY);
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
	printf("Requested bytes: %ld; Read bytes: %ld\n", (long) total, (long) numRead);
	printf("x = %d\n", x);
	printf("str = %s\n", str);
}

int main()
{
	first();
	second();
}
