#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>

int main()
{
	struct utsname uts;
	if (uname(&uts) == -1) {
		perror("uname error");
		exit(EXIT_FAILURE);
	}
	printf("sysname: %s\n", uts.sysname);
	printf("nodename: %s\n", uts.nodename);
	printf("release: %s\n", uts.release);
	printf("version: %s\n", uts.version);
	printf("machine: %s\n", uts.machine);
#ifdef _GNU_SOURCE
	printf("domainnam: %s\n", uts.domainname);
#endif
}
