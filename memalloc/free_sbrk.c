/* test when free() updates program break */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	if (argc < 3 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s num-allocs block-size [step [min [max]]]\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	int numAllocs = (int) strtol(argv[1], NULL, 0);
	int blockSize = (int) strtol(argv[2], NULL, 0);
	int freeStep = (argc > 3) ? (int) strtol(argv[3], NULL, 0) : 1;
	int freeMin = (argc > 4) ? (int) strtol(argv[4], NULL, 0) : 1;
	int freeMax = (argc > 5) ? (int) strtol(argv[5], NULL, 0) : numAllocs;
	printf("initial program break: %p\n", sbrk(0));
	printf("allocating %d*%d bytes\n", numAllocs, blockSize);
	char *ptr[numAllocs];
	for (int i=0; i<numAllocs; i++) {
		ptr[i] = malloc(blockSize);
	}
	printf("Program break is now: %p\n", sbrk(0));
	printf("freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);
	for (int i=freeMin; i<freeMax; i+=freeStep) {
		free(ptr[i]);
	}
	printf("after free(), program break is %p\n", sbrk(0));
	exit(EXIT_SUCCESS);
}
