#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc<2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <filepath>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	DIR *dir = opendir("/proc/");
	if (dir == NULL) {
		perror("opendir error");
		exit(EXIT_FAILURE);
	}
	/* enumerate for directories in /proc */
	for (struct dirent *rdir = readdir(dir); rdir != NULL; rdir = readdir(dir)) {
		char *exess;
		int pidNum = (int) strtol(rdir->d_name, &exess, 10);
		/* if the directory is not a PID directory */
		if (exess[0] != '\0') {
			continue;
		}
		char *pid = rdir->d_name;
		char filename[strlen("/proc/") + strlen(pid) + strlen("/status") + 1];
		/* enumerate /proc/PID/fd */
		char fdirname[1024];
		fdirname[0] = '\0';
		strcat(fdirname, "/proc/");
		strcat(fdirname, pid);
		strcat(fdirname, "/fd");
		DIR *fdir = opendir(fdirname);
		if (fdir == NULL) {
			continue;
		}
		char expanded[2048];
		int match = 0;
		for (struct dirent *frdir = readdir(fdir); frdir != NULL; frdir = readdir(fdir)) {
			char pathname[2048];
			pathname[0] = '\0';
			strcat(pathname, fdirname);
			strcat(pathname, "/");
			strcat(pathname, frdir->d_name);
			int bufsize = readlink(pathname, expanded, 2048);
			if (bufsize == -1)
				continue;
			expanded[bufsize+2] = '\0';
			if (strstr(expanded, argv[1]) != NULL) {
				match = 1;
				break;
			}
		}
		if (match == 0)
			continue;
		/* read stuff from status file */
		filename[0] = '\0';
		strcat(filename, "/proc/");
		strcat(filename, pid);
		strcat(filename, "/status");
		int fd = open(filename, O_RDONLY);
		/* the file might not exist now */
		if (fd == -1)
			continue;
		lseek(fd, 0, SEEK_SET);
		char *name = malloc(2048);
		char buf[65536];
		buf[65535] = '\0';
		for (int readed = read(fd, buf, 65536); readed > 0; readed = read(fd, buf, 65536)) {
			/* find the name of the command this process is running */
			for (int i=0; i<65536; i++) {
				if (buf[i] != '\t')
					continue;
				int idx = 0;
				for (int j=i+1; j<65536; j++) {
					if (buf[j] == '\n')
						break;
					memcpy(name+idx, buf+j, 1);
					//name[idx] = buf[j];
					idx++;
				}
				name[idx] = '\0';
				break;
			}
			/* find this process's parent PID */
			char *location = strstr(buf, "PPid:\t");
			if (location == NULL) {
				printf("could not find ppid\n");
				continue;
			}
			int idx = 0;
			location = location + 6;
			for (; *location != '\n'; location++) {
				buf[idx] = *location;
				idx++;
				buf[idx] = '\0';
			}
		}
		printf("[%d] %s\n", pidNum, name);
	}
}
