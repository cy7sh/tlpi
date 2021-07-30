/* Exercise 5-2 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int fd = open("test", O_WRONLY | O_APPEND);
	if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("seek error");
		exit(EXIT_FAILURE);
	}
	if (write(fd, "hello", 5) == -1) {
		perror("write error");
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
