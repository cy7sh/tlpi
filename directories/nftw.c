#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

enum
{
  FTW_F,		/* Regular file.  */
#define FTW_F	 FTW_F
  FTW_D,		/* Directory.  */
#define FTW_D	 FTW_D
  FTW_DNR,		/* Unreadable directory.  */
#define FTW_DNR	 FTW_DNR
  FTW_NS,		/* Unstatable file.  */
#define FTW_NS	 FTW_NS

  FTW_SL,		/* Symbolic link.  */
# define FTW_SL	 FTW_SL

/* These flags are only passed from the `nftw' function.  */
  FTW_DP,		/* Directory, all subdirs have been visited. */
# define FTW_DP	 FTW_DP
  FTW_SLN		/* Symbolic link naming non-existing file.  */
# define FTW_SLN FTW_SLN
};


/* Flags for fourth argument of `nftw'.  */
enum
{
  FTW_PHYS = 1,		/* Perform physical walk, ignore symlinks.  */
# define FTW_PHYS	FTW_PHYS
  FTW_MOUNT = 2,	/* Report only files on same file system as the
			   argument.  */
# define FTW_MOUNT	FTW_MOUNT
  FTW_CHDIR = 4,	/* Change to current directory while processing it.  */
# define FTW_CHDIR	FTW_CHDIR
  FTW_DEPTH = 8		/* Report files in directory before directory itself.*/
# define FTW_DEPTH	FTW_DEPTH
  ,
  FTW_ACTIONRETVAL = 16	/* Assume callback to return FTW_* values instead of
			   zero to continue and non-zero to terminate.  */
#  define FTW_ACTIONRETVAL FTW_ACTIONRETVAL
};

struct FTW {
	char *base;
	int level;
};

int evaluateNode(const char *dirpath, int flags, int level, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf))
{
	struct stat sb;
	int out;
	int typeflag;
	if (flags & FTW_PHYS) out = lstat(dirpath, &sb);
	else out = stat(dirpath, &sb);
	if (out == -1) {
		switch (errno) {
			case ENOENT:
				typeflag = FTW_SLN;
			default:
				typeflag = FTW_NS;
		}
	}
	else {
		switch (sb.st_mode & S_IFMT) {
			case S_IFREG:
				typeflag = FTW_F;
				break;
			case S_IFDIR:
				if (flags & FTW_DEPTH) typeflag = FTW_DP;
				else typeflag = FTW_D;
				break;
			case S_IFLNK:
				typeflag = FTW_SL;
				break;
		}
	}
	char baseTemp[PATH_MAX];
	strcpy(baseTemp, dirpath);
	char *base = basename(baseTemp);
	struct FTW ftwbuf = {
		.base = base,
		.level = level
	};
	if (fn(dirpath, &sb, typeflag, &ftwbuf) != 0) {
		return 1;
	}
	return 0;
}

int traverseNode(const char *path, int flags, int level, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf))
{
	if (!(flags & FTW_DEPTH)) {
		evaluateNode(path, flags, level, fn);
	}
	DIR *node = opendir(path);
	struct dirent *child = readdir(node);
	while (child != NULL) {
		if (!(strcmp(child->d_name, ".")) || !(strcmp(child->d_name, ".."))) {
			child = readdir(node);
			continue;
		}
		char childPath[PATH_MAX];
		strcpy(childPath, path);
		strcat(childPath, "/");
		strcat(childPath, child->d_name);
		if (child->d_type == DT_DIR) {
			/* recursively traverse directories */
			if (traverseNode(childPath, flags, level+1, fn) != 0) return 1;
		}
		else
			if (evaluateNode(childPath, flags, level, fn) != 0) {
				return 1;
			}
		child = readdir(node);
	}
	/* post-order if FTW_DEPTH is set */
	if ((flags & FTW_DEPTH)) {
		evaluateNode(path, flags, level, fn);
	}
	return 0;
}

int nftw(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags)
{
	int level = 0;
	if (traverseNode(dirpath, flags, level, fn) != 0) return -1;
	return 0;
}

int test(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	switch (typeflag) {
		case FTW_F:
			printf("Regular: %s\n", fpath);
			printf("Base: %s Level: %d\n", ftwbuf->base, ftwbuf->level);
			printf("Inode: %ld\n", sb->st_ino);
			break;
		case FTW_D:
			printf("Directory: %s\n", fpath);
			printf("Base: %s Level: %d\n", ftwbuf->base, ftwbuf->level);
			printf("Inode: %ld\n", sb->st_ino);
			break;
		case FTW_SL:
			printf("Symlink: %s\n", fpath);
			printf("Base: %s Level: %d\n", ftwbuf->base, ftwbuf->level);
			printf("Inode: %ld\n", sb->st_ino);
			break;
	}
	printf("\n");
	return 0;
}

int main(int argc, char *argv[])
{
	if (nftw(argv[1], test, 10, FTW_PHYS) != 0) puts("fail");
}
