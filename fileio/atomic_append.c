/* Exercise 5-3 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 3 || argc > 4 || (strcmp(argv[1], "--help") == 0)) {
		printf("Usage: %s <file> <num-bytes> [x]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int fd;
	if (argc == 4 && strcmp(argv[3], "x") == 0) {
		fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		for (int i = 0; i < atoll(argv[2]); i++) {
			lseek(fd, 0, SEEK_END);
			write(fd, "x", 1);
		}
	} else {
		fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		for (int i = 0; i < atoll(argv[2]); i++) {
			write(fd, "x", 1);
		}
	}
}
