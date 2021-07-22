/* Exercise 6-4 */

/* Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
 * code that directly modifies environ. Your version of unsetenv() should check to see
 * whether there are multiple definitions of an environment variable, and remove
 * them all (which is what the glibc version of unsetenv() does).
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int setenv(const char *name, const char *value, int overwrite)
{
	char toPut[strlen(name) + strlen(value) + 2]; // name + "=" + value + x00
	strcat(toPut, name);
	strcat(toPut, "=");
	strcat(toPut, value);
	puts(toPut);
	int fd = open("test", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	write(fd, toPut, sizeof(toPut));
	if (getenv(name) != NULL && overwrite == 0) {
		return 0;
	}
	if (putenv(toPut) != 0) {
		return -1;
	}
	return 0;
}

int main()
{
	int stat = setenv("SHELL", "yo", 1);
	printf("%d\n", stat);
	puts(getenv("SHELL"));
}
