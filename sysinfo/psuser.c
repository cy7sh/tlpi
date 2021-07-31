#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
	if (argc != 2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <user>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *user = argv[1];
	DIR *dir = opendir("/proc/");
	if (dir == NULL) {
		perror("opendir error");
		exit(EXIT_FAILURE);
	}
	struct passwd *pwd = getpwnam(argv[1]);
	if (pwd == NULL) {
		printf("invalid user\n");
		exit(EXIT_FAILURE);
	}
	uid_t uid = pwd->pw_uid;
	char uidStr[1000000];
	sprintf(uidStr, "%d", uid);

	/* enumerate for directories in /proc */
	for (struct dirent *rdir = readdir(dir); rdir != NULL; rdir = readdir(dir)) {
//		printf("checking dir %s\n", rdir->d_name);
		char *exess;
		strtol(rdir->d_name, &exess, 10);
		/* if the directory is not a PID directory */
		if (exess[0] != '\0') {
//			printf("not pid\n");
			continue;
		}
		/* read stuff from status file */
		char *pid = rdir->d_name;
		char filename[strlen("/proc/") + strlen(pid) + strlen("/status") + 1];
		filename[0] = '\0';
		strcat(filename, "/proc/");
		strcat(filename, pid);
		strcat(filename, "/status");
		int fd = open(filename, O_RDONLY);
		/* the file might not exist now */
		if (fd == -1)
			continue;
//		printf("current file %s\n", filename);
		/* read from status file and check for match */
		char buf[65536];
		/* i know readed is not a word */
		for (int readed = read(fd, buf, 65535); readed > 0; readed = read(fd, buf, 65536)) {
			buf[65535] = '\0';
			char searchTerm[strlen("Uid:\t") + strlen(uidStr) + 2];
			searchTerm[0] = '\0';
			strcat(searchTerm, "Uid:\t");
			strcat(searchTerm, uidStr);
//			printf("searching for '%s'\n", searchTerm);
//			printf("checking PID: %s\n", pid);
			if (strstr(buf, searchTerm) == NULL) continue;
			printf("PID: %s\n", pid);
			break;
		}
	}
}
