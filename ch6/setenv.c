/* Exercise 6-4 */

/* Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
 * code that directly modifies environ. Your version of unsetenv() should check to see
 * whether there are multiple definitions of an environment variable, and remove
 * them all (which is what the glibc version of unsetenv() does).
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int setenv(const char *name, const char *value, int overwrite)
{
	char *toPut = malloc(strlen(name) + strlen(value) + 2); // name + "=" + value + x00
	toPut[0] = '\0';
	strcat(toPut, name);
	strcat(toPut, "=");
	strcat(toPut, value);
	printf("toput is %s\n", toPut);
	if (getenv(name) != NULL && overwrite == 0) {
		return 0;
	}
	if (putenv(toPut) != 0) {
		return -1;
	} else {
		printf("after put: shell: %s, test: %s\n", getenv("SHELL"), getenv("test"));
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
		} else {
			return -1;
		}
	}
	return 0;
}

void printenv()
{
	for (char **ep = environ; *ep != NULL; ++ep) {
		puts(*ep);
	}
}

int main()
{
	puts(getenv("SHELL"));
	setenv("SHELL", "yo", 1);
	setenv("test", "how does this work?", 1);
//	printenv();
	printf("After setenv\n");
	printf("SHELL: %s\n", getenv("SHELL"));
	printf("test: %s\n", getenv("test"));
//	if (unsetenv("SHELL") == -1) {
//		printf("unset fail\n");
//	}
//	if (getenv("SHELL") == NULL) {
//		printf("successfully unset\n");
//	} else {
//		puts(getenv("SHELL"));
//	}
//	puts(getenv("test"));
}
