#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/xattr.h>

extern char *optarg;

int main(int argc, char *argv[])
{
	if (argc > 7 || argc < 4 || !strcmp(argv[1], "--help")) {
		printf("Usage: \n");
		printf("%s [-h] -n name [-v value] pathname\n", argv[0]);
		printf("%s [-h] -x name pathname\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int derefer = 1;
	int rm = 0;
	char *name = NULL;
	char *value = NULL;
	char *pathname = argv[argc-1];
	char opt;
	while ((opt = getopt(argc, argv, "hx:n:v:")) != -1)
	{
		switch (opt) {
		case 'h':
			derefer = 0;
			break;
		case 'n':
			name = malloc(strlen(optarg)+1);
			memcpy(name, optarg, strlen(optarg)+1);
			break;
		case 'v':
			value = malloc(strlen(optarg)+1);
			memcpy(value, optarg, strlen(optarg)+1);
			break;
		case 'x':
			rm = 1;
			name = malloc(strlen(optarg)+1);
			memcpy(name, optarg, strlen(optarg)+1);
			break;
		}
	}
	if (rm == 1) {
		if (derefer) {
			if (removexattr(pathname, name) == -1) {
				perror("failed to remove attribute");
				exit(EXIT_FAILURE);
			}
		}
		if (!derefer) {
			if (lremovexattr(pathname, name) == -1) {
				perror("failed to remove attribute");
				exit(EXIT_FAILURE);
			}
		}
	} else if (derefer) {
		if (setxattr(pathname, name, value, sizeof(value), 0) == -1) {
			perror("failed to set attributes\n");
			exit(EXIT_FAILURE);
		}
	} else {
		if (lsetxattr(pathname, name, value, sizeof(value), 0) == -1) {
			perror("failed to set attributes\n");
			exit(EXIT_FAILURE);
		}
	}
}
