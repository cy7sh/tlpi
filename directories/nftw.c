#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

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
	int base;
	int level;
};

int nftw(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags)
{
	DIR *parent = opendir(dirpath);
	if (!(flags & FTW_DEPTH)) {
		struct stat sb;
		int out;
		int typeflag;
		if (flags & FTW_PHYS) out = lstat(dirpath, &sb);
		else out = stat(dirpath, &sb);
		if (out == -1) typeflag = FTW_NS;
		switch (sb.st_mode & S_IFMT) {
			case S_IFREG:
				typeflag = FTW_F;
				break;
			case S_IFDIR:
				typeflag = FTW_D;
				break;
			case S_IFLNK:
				typeflag = FTW_SL;
				break;
		}
	}
	return 0;
}

int main()
{

}
