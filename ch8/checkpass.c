#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	long lnmax = sysconf(_SC_LOGIN_NAME_MAX);
	if (lnmax == -1)
		lnmax = 256;
	char *username = malloc(lnmax);
	printf("Username: ");
	fflush(stdout);
	fgets(username, lnmax, stdin);
	size_t len = strlen(username);
	if (username[len-1] == '\n')
		username[len-1] = '\0';
	struct passwd *pwd = getpwnam(username);
	if (pwd == NULL) {
		printf("could not find password record\n");
		exit(EXIT_FAILURE);
	}
	struct spwd *spwd = getspnam(username);
	if (spwd == NULL && errno == EACCES) {
		printf("permission error\n");
		exit(EXIT_FAILURE);
	}
	if (spwd != NULL) {
		pwd->pw_passwd = spwd->sp_pwdp;
	}
	char *password = getpass("Password: ");
	char *encrypted = crypt(password, pwd->pw_passwd);
	int authOk = strcmp(encrypted, pwd->pw_passwd);
	if (authOk) {
		printf("incorrect password\n");
		exit(EXIT_FAILURE);
	}
	printf("successfully authenticated (UID: %ld)\n", (long) pwd->pw_uid);
	exit(EXIT_SUCCESS);
}
