#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 100

int main(int argc, char *argv[])
{
	char line[MAX_LINE];
	int fd = open("/proc/sys/kernel/pid_max", (argc > 1) ? O_RDWR : O_RDONLY);
	if (fd == -1) {
		perror("could not open pid_max");
		exit(EXIT_FAILURE);
	}
	ssize_t n = read(fd, line, MAX_LINE);
	if (fd == -1) {
		perror("could not read pid_max");
		exit(EXIT_FAILURE);
	}
	if (argc == 1)
		printf("current pid_max: %.*s\n", (int) n, line);
	if (argc > 1) {
		printf("old pid_max: %.*s\n", (int) n, line);
		/* because we read before */
		lseek(fd, 0, SEEK_SET);
		if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
			perror("write failed");
			exit(EXIT_FAILURE);
		}
	}
	system("echo /proc/sys/kernel/pid_max now contains `cat /proc/sys/kernel/pid_max`");
}
