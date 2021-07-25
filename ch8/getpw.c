#include <stdio.h>
#include <pwd.h>


int main()
{
	struct passwd *pw = getpwuid(0);
	printf("uid: %d name: %s\n", pw->pw_uid, pw->pw_name);
}
