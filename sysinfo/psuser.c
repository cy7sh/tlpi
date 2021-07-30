#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <math.h>

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
		char *exess;
		strtol(rdir->d_name, &exess, 10);
		/* if the directory is not a PID directory */
		if (exess[0] != '\0') {
			continue;
		}
		/* read stuff from status file */
		char filename[strlen("/proc/") + strlen(rdir->d_name) + strlen("/status") + 1];
		char *pid = rdir->d_name;
		strcat(filename, "/proc/");
		strcat(filename, pid);
		strcat(filename, "/status");
		int fd = open(filename, O_RDONLY);
		/* the file might not exist now */
		if (fd == -1)
			continue;
		/* read from status file and check for match */
		char buf[65536];
		for (int readed = read(fd, buf, 65536); readed > 0; readed = read(fd, buf, 65536)) {
			char searchTerm[strlen("Uid:    ") + sizeof(uidStr)];
			strcat(searchTerm, "Uid:    ");
			strcat(searchTerm, uidStr);
			if (strstr(buf, uidStr) == NULL) continue;
			printf("PID: %s\n", pid);
			break;
		}
	}
}
