#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

char *brealpath(const char *path, char *resolved_path)
{
	if (resolved_path == NULL) {
		resolved_path = malloc(PATH_MAX);
	}
	char *cwd;
	if (path[0] != '/') {
		cwd = malloc(PATH_MAX);
		if (getcwd(cwd, PATH_MAX) == NULL) {
			return NULL;
		}
		strcpy(resolved_path, cwd);
		strcat(resolved_path, "/");
		strcat(resolved_path, path);
	} else {
		strcpy(resolved_path, path);
	}
	char *slash = strchr(resolved_path, '/');
	char *prevSlash = NULL;
	while (slash != NULL) {
		/* resolve .. */
		if ((*(slash + 1) == '.') && (*(slash + 2) == '.')) {
			/* 
			 * /foo  /bar  /     .  .  /  foo/bar/
			 *       prev  slash +1 +2 +3 +4 w.r.t slash
			 * /foo/foo/bar/
			 *      prev+1...
			 */
			int j = 1;
			for (char *i = slash+4; *(i-1) != '\0'; i++) {
				*(prevSlash + j) = *i;
				j++;
			}
		}
		/* resolve . */
		else if (*(slash + 1) == '.') {
			/* 
			 * /foo/bar/     .  /  foo/bar/
			 *         slash +1 +2 +3
			 * /foo/bar/foo/bar/
			 *          +1...
			 */
			int j = 1;
			for (char *i = slash+3; *(i-1) != '\0'; i++) {
				*(slash + j) = *i;
				j++;
			}
		}
		prevSlash = slash;
		/* dereference if current directory is a symlink */
		char *link = malloc(PATH_MAX);
		char *unlinked = malloc(PATH_MAX);
		strcpy(link, resolved_path);
		*(link + (slash - resolved_path)) = '\0';
		if (strlen(link) == 0) {
			goto end;
		}
		int size = readlink(link, unlinked, PATH_MAX);
		if (size == -1 && errno != EINVAL) {
			perror("error resolving link");
			exit(EXIT_FAILURE);
		}
		if (size != -1) {
			unlinked[size] = '\0';
			/* if unlinked is not absolute, make it */
			char temp[PATH_MAX];
			if (unlinked[0] != '/') {
				strcpy(temp, unlinked);
				strcpy(unlinked, cwd);
				strcat(unlinked, "/");
				strcat(unlinked, temp);
			}
			/* update resolved_path */
			strcpy(temp, slash);
			strcpy(resolved_path, unlinked);
			strcat(resolved_path, temp);
		}
end:
		free(link);
		free(unlinked);
		slash = strchr(slash+1, '/');
	}
	return resolved_path;
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		puts(brealpath(argv[1], NULL));
	}
}
