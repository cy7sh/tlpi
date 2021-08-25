#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/inotify.h>

#define MAX_WATCH 65536
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX +1))

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
	int wd = inotify_add_watch(inotify, pathPrefix, IN_ALL_EVENTS);
	watches[nextWatch].wd = wd;
	watches[nextWatch].pathname = pathPrefix;
	nextWatch++;
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
			DIR *childDir = opendir(pathname);
			buildWatch(childDir, pathname);
		}
	}
}

void addToWatch(char *pathname)
{

}

int findWatch(int wd)
{
	int index = -1;
	for (int i=0; i<nextWatch; i++) {
		if (watches[i].wd == wd)
			index = i;
	}
	return index;
}

void keepWatch(int log)
{
	char buf[BUF_LEN];
	while(1) {
		int numRead = read(inotify, buf, BUF_LEN);
		if (numRead == 0) {
			puts("fatal: read returned 0\n");
			exit(EXIT_FAILURE);
		}
		if (numRead == -1) {
			perror("read error");
			exit(EXIT_FAILURE);
		}
		/* log events in buffer */
		for (char *p = buf; p < buf + numRead;) {
			struct inotify_event *event = (struct inotify_event *) buf;
			/* find where the event occurred */
			int index = findWatch(event->wd);
			char pathname[PATH_MAX];
			if (index == -1)
				strcpy(pathname, "(unknown)");
			else
				strcpy(pathname, watches[index].pathname);
			if (strlen(event->name) > 1) {
				strcat(pathname, "/");
				strcat(pathname, event->name);
			}
			if (event->mask & IN_ACCESS) dprintf(log, "%s: file was accessed\n", pathname);
			if (event->mask & IN_ATTRIB) dprintf(log, "%s: file metadata changed\n", pathname);
			if (event->mask & IN_CLOSE_WRITE) dprintf(log, "%s: file opened for writing was closed\n", pathname);
			if (event->mask & IN_CLOSE_NOWRITE) dprintf(log, "%s: file opened read-only was closed\n", pathname);
			if (event->mask & IN_CREATE) dprintf(log, "%s: file/directory created inside\n", pathname);
			if (event->mask & IN_DELETE) dprintf(log, "%s: file/directory deleted inside\n", pathname);
			if (event->mask & IN_DELETE_SELF) dprintf(log, "%s: file/directory was deleted\n", pathname);
			if (event->mask & IN_MODIFY) dprintf(log, "%s: file was modified\n", pathname);
			if (event->mask & IN_MOVE_SELF) dprintf(log, "%s: file/directory was moved\n", pathname);
			if (event->mask & IN_MOVED_FROM) dprintf(log, "%s: file moved out of directory\n", pathname);
			if (event->mask & IN_MOVED_TO) dprintf(log, "%s: file was moved into of directory\n", pathname);
			if (event->mask & IN_OPEN) dprintf(log, "%s: file was opened\n", pathname);
			if (event->cookie > 0) dprintf(log, "cookie: %d\n", event->cookie);
			p += sizeof(struct inotify_event) + event->len;
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
				log = open(optarg, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
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
	dprintf(log, "watching %d directories\n", nextWatch);
	keepWatch(log);
}
