#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <stdlib.h>

void displayTime(const char *msg)
{
	if (msg != NULL)
		printf("%s\n", msg);
	long clockTicks = sysconf(_SC_CLK_TCK);
	clock_t clockTime = clock();
	printf("clock says: %ld clock-per-sec (%.2f secs)\n", (long) clockTime, (double) clockTime / CLOCKS_PER_SEC);
	struct tms t;
	times(&t);
	printf("times says: user %.2f sys %.2f\n", (double) t.tms_utime, (double) t.tms_stime);
}

int main(int argc, char *argv[])
{
	printf("CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n", (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));
	displayTime("program start:");
	int numCalls = (argc > 1) ? atoi(argv[1]) : 100000000;
	printf("doing %d loops\n", numCalls);
	for (int i=0; i<numCalls; i++) {
		(void) getppid();
	}
	displayTime("after loop:");
}
