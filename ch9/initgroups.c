/* Exercise 9-3 */

/* binitgroups = better initgroups, our initgroups */

#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int binitgroups(const char *user)
{
	struct passwd *pwd = getpwnam(user);
	gid_t grouplist[NGROUPS_MAX-1];
	/* the primary GID */
	grouplist[0] = pwd->pw_gid;
	unsigned int nextEntry = 1;
	for (struct group *grp = getgrent(); grp != NULL; grp = getgrent()) {
		for (char **mem = grp->gr_mem; *mem != NULL; mem++) {
			if (!strcmp(*mem, user)) {
				grouplist[nextEntry] = grp->gr_gid;
				nextEntry++;
			}
		}
	}
	setgroups(nextEntry, grouplist);
	return 0;
}

int main()
{
	setgroups(0, NULL);
	printf("Before:\n");
	gid_t grouplist[NGROUPS_MAX-1];
	int groups = getgroups(NGROUPS_MAX-1, grouplist);
	printf("After:\n");
	binitgroups("singurty");
	groups = getgroups(NGROUPS_MAX-1, grouplist);
	for (int i=0; i<groups; i++) {
		printf("%d\n", grouplist[i]);
	}
}
