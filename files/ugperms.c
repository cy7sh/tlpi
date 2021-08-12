#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

extern char *optarg;

void checkUser(const char *name)
{

}

void checkGroup(const char *name)
{

}

int main(int argc, char *argv[])
{
	if (argc<3 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s -[ug] <name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	while (getopt(argc, argv, "u:g:") == -1) {
		switch (*optarg) {
			case 'u':
				checkUser(optarg);
				break;
			case 'g':
				checkGroup(optarg);
				break;
		}
	}
}
