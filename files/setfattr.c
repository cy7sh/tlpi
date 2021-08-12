#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	char *name = NULL;
	char *value = NULL;
	char *pathname = argv[argc-1];
	char opt;
	while ((opt = getopt(argc, argv, "hn:v:")) != -1)
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
		}
	}
}
