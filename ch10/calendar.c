#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
	time_t t = time(NULL);
	printf("seconds since the epoch: %ld\n", (long) t);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf("gettimeofday returned %ld secs, %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec);
	struct tm *gmp;
	gmp = gmtime(&t);
	struct tm gm = *gmp; /* save local copy */
	printf("broken down by gettime():\n");
	printf("year=%d mon=%d mday=%d hour=%d min=%d sec=%d\n", gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
	struct tm *locp;
	locp = localtime(&t);
	struct tm loc = *locp;
	printf("broken down by localtime():\n");
	printf("year=%d mon=%d mday=%d hour=%d min=%d sec=%d\n", loc.tm_year, loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
	printf("asctime formats gmtime as: %s", asctime(&gm));
	printf("asctime formats localtime as: %s", asctime(&loc));
	printf("ctime formats time value as: %s", ctime(&t));
	printf("mktime of gettime: %ld\n", mktime(&gm));
	printf("mktime of localtime: %ld\n", mktime(&loc));
}
