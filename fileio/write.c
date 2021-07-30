/* Exercise 5-6 */

/* fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
 * fd2 = dup(fd1);
 * fd3 = open(file, O_RDWR);
 * write(fd1, "Hello,", 6);
 * Wirtes "Hello," at the beginning of the file. File now contains: "Hello,"
 * write(fd2, "world", 6);
 * Writes "world\x00" at the end of the file (where the offset is). File now contains "Hello,world\x00"
 * lseek(fd2, 0, SEEK_SET);
 * Sets offset to beginning of the file
 * write(fd1, "HELLO,", 6);
 * Writes "HELLO," at the begging of the file. File now containes: "HELLO,world\x00"
 * write(fd3, "Gidday", 6);
 * Writes "Gidday" at the begging of the file (where the offset is for this description). File now containes "Giddayworld\x00"
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char file[] = "test";

	int fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	int fd2 = dup(fd1);
	int fd3 = open(file, O_RDWR);
	write(fd1, "Hello,", 6);
	write(fd2, "world", 6);
	lseek(fd2, 0, SEEK_SET);
	write(fd1, "HELLO,", 6);
	write(fd3, "Gidday", 6);
	close(fd1);
	close(fd2);
	close(fd3);
}
