#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
	char buffer[1024];
	ssize_t numRead;
	int tty;
	tty = open("/dev/pts/4", O_RDONLY);
	if (tty == -1) {
		perror("error opening tty");
		exit(EXIT_FAILURE);
	}
	while (*buffer != '\n') {
		numRead = read(tty, buffer, 1023);
		if (numRead == -1) {
			perror("error reading");
			exit(EXIT_FAILURE);
		}
		printf("%s", buffer);
	}
	
	buffer[numRead] = '\0';
	printf("the input was: %s", buffer);
}
