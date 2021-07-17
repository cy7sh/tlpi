#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	if (argc < 3 || (strcmp(argv[1], "--help") == 0)) {
		printf("Usage: %s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR, S_IWUSR, S_IRGRP, S_IWGRP, S_IROTH); /* rw-rw-r */
	if (fd == -1) {
		perror("error opening file");
		exit(EXIT_FAILURE);
	}
	for (int arg = 2; arg < argc; arg++) {
		switch (argv[arg][0]) {
			case 'r':
			case 'R':
				errno = 0;
				char *str;
				size_t len = strtol(&argv[arg][1], &str, 10);
				char *buf = malloc(len);
				if (buf == NULL) {
					perror("malloc error");
					exit(EXIT_FAILURE);
				}
				int fd;
				ssize_t numRead = read(fd, buf, len);
				if (numRead == -1) {
					perror("read error");
					exit(EXIT_FAILURE);
				}
				if (numRead == 0) {
					printf("%s: end-of-file\n", argv[arg]);
				} else {
					printf("%s: ", argv[arg]);
					for (int j = 0; j < numRead; j++) {
						if (argv[arg][0] == 'r') {
							printf("%c", isprint((unsigned char) buf[j]) ? buf[j] : '?');
						} else {
							printf("%02x", (unsigned int) buf[j]);
						}
					}
					printf("\n");
				}
				free(buf);
				break;
			case 'w': ;
				ssize_t numWrittern = write(fd, &argv[arg][1], strlen(&argv[arg][1]));
				if (numWrittern == -1) {
					perror("write error");
					exit(EXIT_FAILURE);
				}
				printf("%s: wrote %ld bytes\n", argv[arg], (long) numWrittern);
				break;
			case 's':
				errno = 0;
				off_t offset = strtol(&argv[arg][1], &str, 10);
				if (lseek(fd, offset, SEEK_SET) == -1) {
					perror("lseek error");
					exit(EXIT_FAILURE);
				}
				printf("%s: seek succeded\n", argv[arg]);
				break;
			default:
				printf("Argument must start with [rRws]: %s\n", argv[arg]);
				exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
