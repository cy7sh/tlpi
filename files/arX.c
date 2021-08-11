/* implements chmod a+rX */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc<2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	struct stat filestat;
	stat(argv[1], &filestat);
	/* path is a directory */
	if (S_ISDIR(filestat.st_mode)) {
		int perms = filestat.st_mode | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
		chmod(argv[1], perms);
		exit(EXIT_SUCCESS);
	}
	/* path is a file and somebody has execute permission */
	if ((filestat.st_mode & S_IXUSR) || (filestat.st_mode & S_IXGRP) || filestat.st_mode & S_IXOTH) {
		int perms = filestat.st_mode | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
		chmod(argv[1], perms);
		exit(EXIT_SUCCESS);
	}
	/* path is a file and nobody has execute permission */
	int perms = filestat.st_mode | S_IRUSR | S_IRGRP | S_IROTH;
	chmod(argv[1], perms);
	exit(EXIT_SUCCESS);
}
