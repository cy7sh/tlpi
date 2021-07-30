/* Exercise 6-4 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int setenv(const char *name, const char *value, int overwrite)
{
	// put this on heap so that it exists after this function returns
	char *toPut = malloc(strlen(name) + strlen(value) + 2); // name + "=" + value + x00
	toPut[0] = '\0';
	strcat(toPut, name);
	strcat(toPut, "=");
	strcat(toPut, value);
	if (getenv(name) != NULL && overwrite == 0) {
		return 0;
	}
	if (putenv(toPut) != 0) {
		return -1;
	}
	return 0;
}

int unsetenv(const char *name)
{
	if (getenv(name) == NULL) {
		return 0;
	}
	size_t len = strlen(name);
	for (char **ep = environ; *ep != NULL; ++ep) {
		if (!strncmp(*ep, name, len) && (*ep)[len] == '=') {
			char **dp = ep;
			while (*dp != NULL) {
				dp[0] = dp[1];
				++dp;
			}
		}
	}
	return 0;
}

int main()
{
	setenv("test1", "yo", 1);
	setenv("test2", "how does this work?", 1);
	printf("After setenv\n");
	printf("test1: %s\n", getenv("test1"));
	printf("test2: %s\n", getenv("test2"));
	if (unsetenv("test1") == -1) {
		printf("unset fail\n");
	}
	if (unsetenv("test2") == -1) {
		printf("unset fail\n");
	}
	printf("After unsetenv\n");
	printf("test1: %s\n", getenv("test1"));
	printf("test2: %s\n", getenv("test2"));
}
