/* Exercise 7-2 */

/* bmalloc = better malloc, our malloc
 * bfree = better free, our free
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FREES 1000
#define NEXT_FREE 0

void *freeList[MAX_FREES];

void *bmalloc(size_t size)
{
	void *programBreak = sbrk(size + sizeof(size_t));
	/* add metadata before the data segment giving the block form:
	 * Length of block + Acutal data
	 */
	memcpy(programBreak, &size, sizeof(size_t));
	/* return just the data segment hiding the metadata */
	return programBreak + sizeof(size_t);
}

void bfree(void *ptr)
{
	ptr = ptr - sizeof(size_t);
	/* save pointer to free block in freeList */
	freeList[NEXT_FREE] = ptr;
	if (NEXT_FREE == 0)
		return;
	/* store address to previous block and next block in the block itself
	 * giving the block form: Length of block + Pointer to previous block
	 * + Pointer to next block + Remaining block
	 */
	memcpy(ptr + sizeof(size_t), (void *) freeList[NEXT_FREE-1], sizeof(void *));
	/* we don't know what the next block will be yet */
	memcpy(ptr + sizeof(size_t) + sizeof(void ()), (void *) NULL, sizeof(void *));
	/* update the "next pointer" of previous block */
	void *prevPtr = freeList[NEXT_FREE-1];
	memcpy(prevPtr + sizeof(size_t) + sizeof(void ()), (void *) ptr, sizeof(void *));
}

int main()
{
	size_t memSize = 1024;
	void *addr = bmalloc(memSize);
	printf("addr is %p\n", addr);
	void *addr2 = bmalloc(memSize);
	printf("addr2 is %p\n", addr2);
	void *addr3 = bmalloc(memSize);
	printf("addr3 is %p\n", addr3);
	void *addr4 = bmalloc(memSize);
	printf("addr4 is %p\n", addr4);
	bfree(addr);
}
