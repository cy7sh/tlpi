#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>
#include <unistd.h>

/* one directory is the directory to traverse itself */
int regular = 0, directory = -1, symbolic = 0, unknown = 0;

int populateCounts(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	switch (typeflag) {
		case FTW_F:
			regular++;
			break;
		case FTW_D:
		case FTW_DNR:
		case FTW_DP:
			directory++;
			break;
		case FTW_NS:
			unknown++;
			break;
		case FTW_SL:
		case FTW_SLN:
			symbolic++;
			break;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (nftw(argv[1], &populateCounts, sysconf(_SC_OPEN_MAX), FTW_PHYS) == 0) {
		puts("directory traversal finished");
	}
	float sum = regular + directory + symbolic + unknown;
	printf("Regular files: %.2f%% %d)\n", (regular / sum) * 100, regular);
	printf("Directory files: %.2f%% %d)\n", (directory / sum) * 100, directory);
	printf("Symbolic files: %.2f%% %d)\n", (symbolic / sum) * 100, symbolic);
	printf("Unknown files: %.2f%% %d)\n", (unknown / sum) * 100, unknown);
}
