#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

char *getcwd(char *buf, size_t size)
{
	if (buf == NULL) {
		if (size > 0) {
			buf = malloc(size);
		} else {
			buf = malloc(PATH_MAX);
		}
	}
	buf[0] = '\0';
	int initFd = open(".", O_RDONLY);
	while (1) {
		struct stat cwdStat;
		stat(".", &cwdStat);
		/* check if we reached the top */
		struct stat parentStat;
		stat("..", &parentStat);
		if (parentStat.st_ino == cwdStat.st_ino && parentStat.st_dev == cwdStat.st_dev) break;
		DIR *parent = opendir("..");
		struct dirent *child = readdir(parent);
		struct stat childStat;
		while (child != NULL) {
			char childPath[PATH_MAX];
			strcpy(childPath, "../");
			strcat(childPath, child->d_name);
			if (stat(childPath, &childStat) == -1) {
				perror("stat error");
				exit(EXIT_FAILURE);
			}
			if (childStat.st_ino == cwdStat.st_ino && childStat.st_dev == cwdStat.st_dev) {
				char temp[PATH_MAX];
				strcpy(temp, buf);
				strcpy(buf, child->d_name);
				strcat(buf, "/");
				strcat(buf, temp);
			}
			child = readdir(parent);
		}
		chdir("..");
	}
	char temp[PATH_MAX];
	strcpy(temp, buf);
	strcpy(buf, "/");
	strcat(buf, temp);
	return buf;
}

int main()
{
	puts(getcwd(NULL, 0));
}
