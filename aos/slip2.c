// Q1

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
void main(int argc, char *argv[])
{
	struct dirent *de;
	if (argc != 2)
	{
		fprintf(stderr, "usage : %s <search string>\n", argv[0]);
		return;
	}
	DIR *directory = opendir(".");
	char **filenames;
	if (directory == NULL)
	{
		printf("Could not open current directory");
		return;
	}
	char *searchOut;

	rewinddir(directory);
	while ((de = readdir(directory)) != NULL)
		if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
		{
			searchOut = strstr(de->d_name, argv[1]);
			int index = searchOut - de->d_name;
			if (index == 0)
			{
				printf("%s\n", de->d_name);
			}
		}
	closedir(directory);
}

// Q2

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
static jmp_buf jmpbuffer;
static int globval;
static void f1(int i, int j, int k, int l)
{
	printf("in f1():\n");
	printf("globval = %d, autoval = %d, regival = %d,"
		   " volaval = %d, statval = %d\n",
		   globval, i, j, k, l);
	f2();
}
static void f2(void)

{
	longjmp(jmpbuffer, 1);
}
int main(void)
{
	int autoval;
	register int regival;
	volatile int volaval;
	static int statval;
	globval = 1;
	autoval = 2;
	regival = 3;
	volaval = 4;
	statval = 5;
	if (setjmp(jmpbuffer) != 0)
	{
		printf("after longjmp:\n");
		printf("globval = %d, autoval = %d, regival = %d,"
			   " volaval = %d, statval = %d\n",
			   globval, autoval, regival, volaval, statval);
		exit(0);
	}
	/*
	 * Change variables after setjmp, but before longjmp.
	 */
	globval = 95;
	autoval = 96;
	regival = 97;
	volaval = 98;
	statval = 99;
	f1(autoval, regival, volaval, statval); /* never returns */
	exit(0);
}