#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
	char **ep;
	for (ep=environ; *ep != NULL; ep++){
		puts(*ep);
	}
	//environ = NULL;
	clearenv();
	putenv("SHELL=yo bitch");
	for (ep=environ; *ep != NULL; ep++){
		puts(*ep);
	}
	char *SHELL = getenv("SHELL");
	puts(SHELL);
	unsetenv("SHELL");
	SHELL = getenv("SHELL");
	puts(SHELL);
}
