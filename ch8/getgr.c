#include <stdio.h>
#include <grp.h>

int main()
{
	struct group *grp = getgrnam("sudo");
	char **members = grp->gr_mem;
	for (; *members != NULL; members++) {
		printf("%s\n", *members);
	}
}
