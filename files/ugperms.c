#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/acl.h>

extern char *optarg;

void checkUser(const char *name, acl_entry_t *entry)
{

}

void checkGroup(const char *name, acl_entry_t *entry)
{

}

int main(int argc, char *argv[])
{
	if (argc<4 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s -[ug] <name> <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	acl_t acl = acl_get_file(argv[argc-1], ACL_TYPE_ACCESS);
	int user = -1;
	char *name;
	while (getopt(argc, argv, "u:g:") == -1) {
		switch (*optarg) {
			case 'u':
				user = 1;
				name = malloc(strlen(optarg) + 1);
				memcpy(name, optarg, strlen(optarg)+1);
				break;
			case 'g':
				user = 0;
				name = malloc(strlen(optarg) + 1);
				memcpy(name, optarg, strlen(optarg)+1);
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
			checkUser(name, &entry);
		} else {
			checkGroup(name, &entry);
		}
		status = acl_get_entry(acl, ACL_NEXT_ENTRY, &entry);
	} while (status != 0 && status != -1);
}
