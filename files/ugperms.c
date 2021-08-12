#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/acl.h>
#include <acl/libacl.h>
#include <pwd.h>
#include <grp.h>

extern char *optarg;

void checkUser(const uid_t *uid, acl_entry_t *entry)
{
	acl_tag_t tagType;
	if (acl_get_tag_type(*entry, &tagType) == -1) {
		printf("error reading entry\n");
		exit(EXIT_FAILURE);
	}
	if (tagType == ACL_USER_OBJ || tagType == ACL_USER) {
		uid_t *aclUid = acl_get_qualifier(*entry);
		if (aclUid == NULL) {
			perror("failed to fetch qualifier");
			exit(EXIT_FAILURE);
		}
		if (aclUid == uid) {
			acl_permset_t permset;
			if (acl_get_permset(*entry, &permset) == -1) {
				perror("failed to fetch permission set");
				exit(EXIT_FAILURE);
			}
			if (acl_get_perm(permset, ACL_READ) == 1) {
				puts("read allowed");
			}
			if (acl_get_perm(permset, ACL_WRITE) == 1) {
				puts("write allowed");
			}
			if (acl_get_perm(permset, ACL_EXECUTE) == 1) {
				puts("execute allowed");
			}
		}
	}
}

void checkGroup(const gid_t *gid, acl_entry_t *entry)
{
	acl_tag_t tagType;
	if (acl_get_tag_type(*entry, &tagType) == -1) {
		printf("error reading entry\n");
		exit(EXIT_FAILURE);
	}
	if (tagType == ACL_GROUP_OBJ || tagType == ACL_GROUP) {
		gid_t *aclGid = acl_get_qualifier(*entry);
		if (aclGid == NULL) {
			perror("failed to fetch qualifier");
			exit(EXIT_FAILURE);
		}
		if (aclGid == gid) {
			acl_permset_t permset;
			if (acl_get_permset(*entry, &permset) == -1) {
				perror("failed to fetch permission set");
				exit(EXIT_FAILURE);
			}
			if (acl_get_perm(permset, ACL_READ) == 1) {
				puts("read allowed");
			}
			if (acl_get_perm(permset, ACL_WRITE) == 1) {
				puts("write allowed");
			}
			if (acl_get_perm(permset, ACL_EXECUTE) == 1) {
				puts("execute allowed");
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc<4 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s -[ug] <name/id> <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	acl_t acl = acl_get_file(argv[argc-1], ACL_TYPE_ACCESS);
	int user = -1;
	char *name;
	uid_t uid;
	gid_t gid;
	while (getopt(argc, argv, "u:g:") == -1) {
		switch (*optarg) {
			case 'u':
				user = 1;
				char *endptr = malloc(strlen(optarg)+1);
				uid = strtol(optarg, &endptr, 10);
				if (*endptr == '\0')
					break;
				struct passwd *pwd = getpwnam(optarg);
				uid = pwd->pw_uid;
				break;
			case 'g':
				user = 0;
				endptr = malloc(strlen(optarg)+1);
				gid = strtol(optarg, &endptr, 10);
				if (*endptr == '\0')
					break;
				struct group *grp = getgrnam(optarg);
				gid = grp->gr_gid;
				break;
		}
	}
	if (user == -1) {
		printf("invalid flags or arguments\n");
		exit(EXIT_FAILURE);
	}
	acl_entry_t entry;
	int status = acl_get_entry(acl, ACL_FIRST_ENTRY, &entry);
	if (status == -1 || status == 0) {
		printf("couldn't fetch ACL entries\n");
		exit(EXIT_FAILURE);
	}
	do {
		if (user) {
			checkUser(&uid, &entry);
		} else {
			checkGroup(&gid, &entry);
		}
		status = acl_get_entry(acl, ACL_NEXT_ENTRY, &entry);
	} while (status != 0 && status != -1);
}
