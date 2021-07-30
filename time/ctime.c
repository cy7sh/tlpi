#include <stdio.h>
#include <time.h>

int main()
{
	time_t timeUnix = time(NULL);
	char *timePretty = ctime(&timeUnix);
	printf("%s", timePretty);
}
