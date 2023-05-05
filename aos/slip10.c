// Q1

#include <malloc.h>
#include <stdio.h>
void main()
{
	struct mallinfo minfo;
	minfo = mallinfo();
	printf("Memory Allocation Statistics \n");
	printf("Non-mmapped space allocated (bytes) : %d\n", minfo.arena);
	printf("Number of free chunks : %d\n", minfo.ordblks);

	printf("Number of free fastbin blocks : %d\n", minfo.smblks);
	printf("Number of mmapped regions : %d\n", minfo.hblks);
	printf("Space allocated in mmapped regions (bytes) : %d\n", minfo.hblkhd);
	printf("Maximum total allocated space (bytes) : %d\n", minfo.usmblks);
	printf("Space in freed fastbin blocks (bytes) : %d\n", minfo.fsmblks);
	printf("Total allocated space (bytes) : %d\n", minfo.uordblks);
	printf("Total free space (bytes) : %d\n", minfo.fordblks);
	printf("Top-most, releasable space (bytes) : %d\n", minfo.keepcost);
}

// Q2

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
void main(int argc, char **argv)
{
	umask(0000);
	int fd1 = creat("first.txt", S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP |
									 S_IROTH | S_IWOTH);
	int fd2 = creat("second.txt", S_IWUSR | S_IRUSR);
	struct stat st1;
	struct stat st2;
	if (fstat(fd1, &st1) < 0 || fstat(fd2, &st2) < 0)
	{
		fprintf(stderr, "Error reading file stat\n");

		exit(1);
	}
	chmod("first.txt", (st1.st_mode | S_ISGID) & ~S_IXGRP);
	chmod("second.txt", st2.st_mode | S_IROTH);
	close(fd1);
	close(fd2);
}