#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 65536

int main(int argc, char *argv[])
{
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s [-n <number of lines>] <file>\n", argv[0]);
	}
	char *filename = (argc > 3) ? argv[2] : argv[1];
	int toRead = (argc > 3) ? strtol(argv[2], NULL, 10) : 10;
	int fd = open(filename, O_RDONLY);
	/* read from file */
	char buf[BUF_SIZE];
	int numLines = 0;
	for (int numRead = read(fd, buf, BUF_SIZE); numRead != 0; numRead = read(fd, buf, BUF_SIZE)) {
		if (numRead == -1) {
			perror("read error");
			exit(EXIT_FAILURE);
		}
		for (int i=0; i<numRead; i++) {
			if (buf[i] == '\n')
				numLines++;
		}
	}
//	printf("total lines is %d\n", numLines);
	lseek(fd, 0, SEEK_SET);
	int startLine = numLines - toRead + 1;
	for (int numRead = read(fd, buf, BUF_SIZE-1); numRead != 0; numRead = read(fd, buf, BUF_SIZE-1)) {
		if (numRead == -1) {
			perror("read error");
			exit(EXIT_FAILURE);
		}
		int count = 0;
		int doneLoop = 0;
		for (int i=0; i<numRead; i++) {
			if (buf[i] == '\n') {
				count++;
//				printf("current line: %d\n", count);
				if (count == startLine) {
					buf[numRead + 1] = '\0';
					printf("%s", &buf[i]);
					break;
				}
			}
		}
	}
}
