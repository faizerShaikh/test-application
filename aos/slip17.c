// Q1

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <displaymessage> <n>\n", argv[0]);
		return 1;
	}
	int n = atoi(argv[2]);
	jmp_buf environment;
	setjmp(environment);
	if (n == 0)
	{
		return 0;
	}
	n--;
	printf("%s\n", argv[1]);
	longjmp(environment, n);
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
void main(int argc, char **argv)
{
	struct dirent *de;
	struct stat fstat;
	struct tm *timeinfo;
	if (argc != 2)
	{
		printf("no month value passed : pass 0-11 month value\n");
		exit(1);
	}
	int month = atoi(argv[1]);
	if (month < 0 || month > 11)
	{
		printf("invalid month value : pass 0-11 month value\n");
		exit(1);
	}
	DIR *directory = opendir(".");
	char **filenames;
	if (directory == NULL)
	{
		printf("Could not open current directory");
		return;
	}
	while ((de = readdir(directory)) != NULL)
		if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
		{
			stat(de->d_name, &fstat);
			timeinfo = localtime(&fstat.st_ctime);

			if (timeinfo->tm_mon == month)
			{
				printf("%s\n", de->d_name);
			}
		}
	closedir(directory);
}