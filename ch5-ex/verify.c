/* Exercise 5-5 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int firstFd = open("test", O_RDWR | O_CREAT | O_APPEND);
	int secondFd = dup(firstFd);
	int flags = fcntl(secondFd, F_GETFL);
	if (flags & O_APPEND) {
		printf("O_APPEND is set\n");
	}
	if (flags & O_CREAT) {
		/* only file status flags are returned by fcntl so this shouldn't run because O_CREAT is a file creation flag */
		printf("O_CREAT is set\n");
	}
	int accessMode = flags & O_ACCMODE;
	if (accessMode == O_RDWR) {
		printf("O_RDWR is set\n");
	}
}
