// q1

#include <stdio.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	struct rlimit old_lim, lim, new_lim;
	// Get old limits
	if (getrlimit(RLIMIT_NOFILE, &old_lim) == 0)

		printf("Old limits -> soft limit= %ld \t"
			   " hard limit= %ld \n",
			   old_lim.rlim_cur,
			   old_lim.rlim_max);
	else
		fprintf(stderr, "%s\n", strerror(errno));
	// Set new value
	lim.rlim_cur = 3;
	lim.rlim_max = 1024;
	// Set limits
	if (setrlimit(RLIMIT_NOFILE, &lim) == -1)
		fprintf(stderr, "%s\n", strerror(errno));
	// Get new limits
	if (getrlimit(RLIMIT_NOFILE, &new_lim) == 0)
		printf("New limits -> soft limit= %ld "
			   "\t hard limit= %ld \n",
			   new_lim.rlim_cur,
			   new_lim.rlim_max);
	else
		fprintf(stderr, "%s\n", strerror(errno));
	return 0;
}

// Q2

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
typedef struct file_info
{
	char *name;
	size_t size;
} fileinfo;
void insertionSort(fileinfo info[], int n)
{
	int i, j;
	fileinfo key;
	for (i = 1; i < n; i++)
	{
		key = info[i];
		j = i - 1;
		while (j >= 0 && info[j].size > key.size)
		{
			info[j + 1] = info[j];
			j = j - 1;
		}
		info[j + 1] = key;
	}
}
void main(int argc, char **argv)

{
	struct stat fstat;
	if (argc < 2)
	{
		printf("no files passed\n");
		exit(1);
	}
	int fileCount = argc - 1;
	fileinfo info[fileCount];
	int i;
	for (i = 1; i < argc; i++)
	{
		info[i - 1].name = argv[i];
		stat(argv[i], &fstat);
		info[i - 1].size = fstat.st_size;
	}
	insertionSort(info, fileCount);
	for (i = 0; i < fileCount; i++)
	{
		printf("%s -> %ld\n", info[i].name, info[i].size);
	}
}