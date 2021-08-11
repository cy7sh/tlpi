#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h> 

int main(int argc, char *argv[])
{
	if (argc<3 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <mode> <file>\n", argv[1]);
		printf("Format of mode is: +-=[aAcdDijsStTu]\n");
		exit(EXIT_FAILURE);
	}
	int fd = open(argv[2], O_RDONLY);
	int attr = 0;
	for (int i=1; i<strlen(argv[1]); i++) {
		switch (argv[1][i]) {
			case 'a':
				attr |= FS_APPEND_FL;
				break;
			case 'A':
				attr |= FS_NOATIME_FL;
				break;
			case 'c':
				attr |= FS_COMPR_FL;
				break;
			case 'd':
				attr |= FS_NODUMP_FL;
				break;
			case 'D':
				attr |= FS_DIRSYNC_FL;
				break;
			case 'i':
				attr |= FS_IMMUTABLE_FL;
				break;
			case 'j':
				attr |= FS_JOURNAL_DATA_FL;
				break;
			case 't':
				attr |= FS_COMPR_FL;
				attr |= FS_NOTAIL_FL;
				break;
			case 'T':
				attr |= FS_TOPDIR_FL;
				break;
			case 's':
				attr |= FS_SECRM_FL;
				break;
			case 'S':
				attr |= FS_SYNC_FL;
				break;
			case 'u':
				attr |= FS_UNRM_FL;
				break;
			default:
				printf("invalide attribute option\n");
				exit(EXIT_FAILURE);
		}
	}
	int prevAttr = 0;
	switch(argv[1][0]) {
		case '=':
			if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
				perror("failed to set flags");
				exit(EXIT_FAILURE);
			}
			break;
		case '+':
			;
			if (ioctl(fd, FS_IOC_GETFLAGS, &prevAttr) == -1) {
				perror("failed to fetch flags");
				exit(EXIT_FAILURE);
			}
			attr |= prevAttr;
			if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
				perror("failed to set flags");
				exit(EXIT_FAILURE);
			}
			break;
		case '-':
			;
			if (ioctl(fd, FS_IOC_GETFLAGS, &prevAttr) == -1) {
				perror("failed to fetch flags");
				exit(EXIT_FAILURE);
			}
			attr = attr & ~prevAttr;
			if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
				perror("failed to set flags");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			printf("invalide attribute option\n");
			exit(EXIT_FAILURE);
			break;
	}
}
