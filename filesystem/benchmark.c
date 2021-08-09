#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>
#include <errno.h>
#ifdef __FreeBSD__
	#include <sys/stat.h>
#endif

#ifdef _XOPEN_SOURCE
	#include <wordexp.h>
#else
	int dirlen;
#endif

int compare(const void *first, const void *second)
{
	char *firstStr = *(char **) first;
	char *secondStr = *(char **) second;
	if (secondStr == NULL)
		return 1;
	if (firstStr == NULL)
		return -1;
	int firstNum = atoi(&firstStr[dirlen+2]);
	int secondNum = atoi(&secondStr[dirlen+2]);
	if (firstNum == secondNum)
		return 0;
	if (firstNum < secondNum)
		return -1;
	if (firstNum > secondNum)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 3 || !strcmp(argv[0], "--help")) {
		printf("Usage: %s <directory> <number of files>\n", argv[0]);
		printf("Creates <number of files> 1-byte files in <directory> in random order and deletes them in ascending order. Times those operations.\n");
		exit(EXIT_FAILURE);
	}
	int numFiles = strtol(argv[2], NULL, 10);
	if (numFiles > 999999) {
		printf("maximum number of files is 999999\n");
		exit(EXIT_FAILURE);
	}
#ifdef _XOPEN_SOURCE
	wordexp(argv[1], &directory, 0);
	int dirlen = strlen(*directory.we_wordv)
	char filename[dirlen + 9];
#else
	char filename[strlen(argv[1]) + 9];
	dirlen = strlen(argv[1]);
	filename[0] = '\0';
	strcat(filename, argv[1]);
#endif
	strcat(filename, "/");
	strcat(filename, "x");
	struct timeval init;
	struct timeval afterOp;
	char *files[numFiles];
	int next = 0;
	gettimeofday(&init, NULL);
	/* create 1-byte files */
	for (int i=0; next<numFiles; i++) {
		srand(init.tv_usec + i);
		int num = rand() % 999999;
		char numStr[7];
		sprintf(numStr, "%d", num);
		filename[dirlen+2] = '\0';
		strcat(filename, numStr);
		int fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1) {
			/* random numbers are overlapping for some reason */
			if (errno == EEXIST)
				continue;
			perror("file create error");
			exit(EXIT_FAILURE);
		}
		write(fd, "x", 1);
		close(fd);
		char *entry = malloc(sizeof(filename));
		memcpy(entry, filename, sizeof(filename));
		files[next] = entry;
		next++;
	}
	gettimeofday(&afterOp, NULL);
	float interval = (((afterOp.tv_sec * 1000000) + afterOp.tv_usec) - ((init.tv_sec * 1000000) + init.tv_usec)) * 0.000001;
	printf("time taken to create files: %.03f\n", interval);
	/* enumerate the array and delete the files */
	gettimeofday(&init, NULL);
	qsort(files, next, sizeof(char *), compare);
	for (int i=0; i < next; i++) {
		if (remove(files[i]) == -1) {
			perror("error deleting file");
		}
	}
	gettimeofday(&afterOp, NULL);
	interval = (((afterOp.tv_sec * 1000000) + afterOp.tv_usec) - ((init.tv_sec * 1000000) + init.tv_usec)) * 0.000001;
	printf("time taken to delete files: %.03f\n", interval);
}
