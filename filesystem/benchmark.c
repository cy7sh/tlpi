#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wordexp.h>

int main(int argc, char *argv[])
{
	if (argc < 3 || !strcmp(argv[0], "--help")) {
		printf("Usage: %s <directory> <numer of files>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int numFiles = strtol(argv[2], NULL, 10);
	wordexp_t directory;
	wordexp(argv[1], &directory, 0);
	for (int i=0; i<numFiles; i++) {
		srand(i);
		int num = rand() % 999999;
		char numStr[7];
		sprintf(numStr, "%d", num);
		char filename[strlen(*directory.we_wordv) + 9];
		filename[0] = '\0';
		strcat(filename, *directory.we_wordv);
		strcat(filename, "/");
		strcat(filename, "x");
		strcat(filename, numStr);
		puts(filename);
	}
}
