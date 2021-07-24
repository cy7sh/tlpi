/* Exercise 7-2 */

/* bmalloc = better malloc, our malloc
 * bfree = better free, our free
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void *bmalloc(size_t size)
{
	void *programBreak = sbrk(size + sizeof(size_t));
	printf("before wrote size %ld\n", *(size_t *) programBreak);
	memcpy(programBreak, &size, sizeof(size_t));
	printf("wrote size %ld\n", *(size_t *) programBreak);
	return programBreak + sizeof(size_t);
}

void bfree(void *ptr)
{
	size_t size = *(size_t *) (ptr - sizeof(size_t));
	printf("size is %ld\n", size);
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
