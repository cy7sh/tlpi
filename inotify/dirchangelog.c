#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/inotify.h>

#define MAX_WATCH 65536

extern char *optarg;

/* THE inotify nstance */
int inotify;

/* store watch descriptor to pathname mapping */
struct watch {
	int wd;
	char *pathname;
};

struct watch watches[MAX_WATCH];
int nextWatch = 0;

void buildWatch(DIR *directory, char *pathPrefix)
{
	errno = 0;
	struct dirent *dir;
	while ((dir = readdir(directory)) != NULL) {
		if (errno != 0) {
			perror("directory read error");
			exit(EXIT_FAILURE);
		}
		/* ignore . and .. */
		if (!strcmp(dir->d_name, "..") || !strcmp(dir->d_name, "."))
			continue;
		/* create watch and enumerate for subdirectories if it is a directory */
		if (dir->d_type & DT_DIR) {
			char *pathname = malloc(PATH_MAX);
			strcpy(pathname, pathPrefix);
			strcat(pathname, "/");
			strcat(pathname, dir->d_name);
			int wd = inotify_add_watch(inotify, pathname, IN_ALL_EVENTS);
			watches[nextWatch].wd = wd;
			watches[nextWatch].pathname = pathname;
			nextWatch++;
			DIR *childDir = opendir(pathname);
			buildWatch(childDir, pathname);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
		printf("Usage: %s [-f <output file>] <directory>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	inotify = inotify_init();
	int log = STDOUT_FILENO;
	DIR *directory = NULL;
	char opt;
	char *pathPrefix;
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				directory = opendir(argv[3]);
				log = open(optarg, O_RDWR);
				pathPrefix = argv[3];
		}
	}
	if (directory == NULL) {
		directory = opendir(argv[1]);
		pathPrefix = argv[1];
	}
	/* remove traliging slash */
	if (!strcmp(&pathPrefix[strlen(pathPrefix)-1], "/"))
		pathPrefix[strlen(pathPrefix)-1] = '\0';
	buildWatch(directory, pathPrefix);
	/* check if watches are created */
	for (int i=0; i<nextWatch; i++) {
		printf("wd: %d pathname: %s\n", watches[i].wd, watches[i].pathname);
	}
}
