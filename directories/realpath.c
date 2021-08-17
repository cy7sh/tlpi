#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>

char *brealpath(const char *path, char *resolved_path)
{
	if (resolved_path == NULL) {
		resolved_path = malloc(PATH_MAX);
	}
	if (path[0] != '/') {
		char *cwd = malloc(PATH_MAX);
		if (getcwd(cwd, PATH_MAX) == NULL) {
			return NULL;
		}
		resolved_path = cwd;
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
