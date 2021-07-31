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
		printf("Show PID and command name of all process owned by a particular user\n\n");
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
		/* read from status file and check for match */
		char buf[65536];
		buf[65535] = '\0';
		int match = 0;
		/* i know readed is not a word */
		for (int readed = read(fd, buf, 65535); readed > 0; readed = read(fd, buf, 65535)) {
			char searchTerm[strlen("Uid:\t") + strlen(uidStr) + 2];
			searchTerm[0] = '\0';
			strcat(searchTerm, "Uid:\t");
			strcat(searchTerm, uidStr);
			if (strstr(buf, searchTerm) == NULL) continue;
			printf("PID: %s", pid);
			match = 1;
			break;
		}
		if (match == 0)
			continue;
		/* find the name of the command this process is running */
		lseek(fd, 0, SEEK_SET);
		char name[1024];
		for (int readed = read(fd, buf, 65536); readed > 0; readed = read(fd, buf, 65536)) {
			for (int i=0; i<65536; i++) {
				if (buf[i] != '\t')
					continue;
				int idx = 0;
				for (int j=i+1; j<65536; j++) {
					if (buf[j] == '\n')
						break;
					name[idx] = buf[j];
					idx++;
				}
				name[idx] = '\0';
				break;
			}
		}
		printf(" Name: %s\n", name);
	}
}
