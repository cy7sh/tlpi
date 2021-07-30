/* Exercise 5-4 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int newFd = dup(STDOUT_FILENO);
	if (newFd == -1) {
		perror("dup error");
		exit(EXIT_FAILURE);
	}
	printf("STDOUT is now %d\n", newFd);
	write(newFd, "Hello STDOUT\n", 13);
	newFd = dup2(STDOUT_FILENO, 2);
	printf("STDOUT is now %d\n", newFd);
	write(newFd, "Hello STDOUT\n", 13);
	newFd = dup2(STDOUT_FILENO, 1);
	printf("STDOUT is now %d\n", newFd);
	write(newFd, "Hello STDOUT\n", 13);
	newFd = dup2(432424, 1);
	printf("STDOUT is now %d\n", newFd);
	write(newFd, "Hello STDOUT\n", 13);
	exit(EXIT_SUCCESS);
}

int dup(int oldFd)
{
	return fcntl(oldFd, F_DUPFD, 0);
}

int dup2(int oldFd, int newFd)
{
	if (fcntl(oldFd, F_GETFL) == -1) {
		perror("invalid file descriptor");
		exit(EXIT_FAILURE);
	}
	if (oldFd == newFd) {
		return oldFd;
	}
	close(newFd);
	return fcntl(oldFd, F_DUPFD, newFd);
}
