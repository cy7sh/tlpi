#include <stdio.h>
#include <pwd.h>
#include <grp.h>

char* userNameFromId(uid_t uid)
{
	struct passwd *pwd = getpwuid(uid);
	return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t userIdFromName(char *name)
{
	struct passwd *pwd = getpwnam(name);
	return (pwd == NULL) ? -1 : pwd->pw_uid;
}

char* groupNameFrodId(gid_t gid)
{
	struct group *grp = getgrgid(gid);
	return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t groupIdFromName(char *name)
{
	struct group *grp = getgrnam(name);
	return (grp == NULL) ? -1 : grp->gr_gid;
}
