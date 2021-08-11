#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int eaccess(const char *pathname, int mode)
{
	int fd = open(pathname, O_RDONLY);
	if (fd == -1)
		return -1;
	if (mode == F_OK)
		return 0;
	int euid = geteuid();
	int egid = getegid();
	struct stat filestat;
	if (fstat(fd, &filestat) == -1)
		return -1;
	if (euid == filestat.st_uid) {
		if ((mode & R_OK) && !(filestat.st_mode & S_IRUSR))
			return -1;
		if ((mode & W_OK) && !(filestat.st_mode & S_IWUSR))
			return -1;
		if ((mode & X_OK) && !(filestat.st_mode & S_IXUSR))
			return -1;
	} else if (egid == filestat.st_gid) {
		if ((mode & R_OK) && !(filestat.st_mode & S_IRGRP))
			return -1;
		if ((mode & W_OK) && !(filestat.st_mode & S_IWGRP))
			return -1;
		if ((mode & X_OK) && !(filestat.st_mode & S_IXGRP))
			return -1;
	} else {
		if ((mode & R_OK) && !(filestat.st_mode & S_IROTH))
			return -1;
		if ((mode & W_OK) && !(filestat.st_mode & S_IWOTH))
			return -1;
		if ((mode & X_OK) && !(filestat.st_mode & S_IXOTH))
			return -1;
	}
	return 0;
}

int main()
{
	int res = eaccess("test", W_OK | X_OK);
	if (res == 0)
		printf("allowed\n");
}
