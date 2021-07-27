/* Exercise 8-2 */

/* Implement getpwnam() using setpwent(), getpwent(), and endpwent().*/

/* bgetpwnam = better getpwnam, our getpwnam */

#include <stdio.h>
#include <pwd.h>
#include <string.h>

struct passwd *bgetpwnam(char *name)
{
	setpwent();
	while (1) {
		struct passwd* pwd = getpwent();
		if (pwd == NULL)
			return NULL;
		if (!strcmp(pwd->pw_name, name)) {
			return pwd;
		}
	}
}

int main()
{
	struct passwd* pwd = bgetpwnam("root");
	if (pwd != NULL)
		printf("Found: %s\n", pwd->pw_name);
}
