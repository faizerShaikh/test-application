// Q1

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
int file_exist(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void main()
{
	int N = file_exist("abc.txt");
	if (N == 1)
	{
		printf("file does exist in current directory");
	}
	else
	{

		printf("file does not exist in current directory");
	}
}

// Q2

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
	sigset_t newmask, oldmask, pendmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
	{
		printf("SIG_BLOCK error");
		exit(1);
	}
	sleep(5);
	if (sigpending(&pendmask) < 0)
	{
		printf("sigpending error\n");
		exit(1);
	}
	if (sigismember(&pendmask, SIGQUIT))
	{
		printf("SIGQUIT Pending\n");
	}
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		printf("SIG_SETMASK error\n");

		exit(1);
	}
	sleep(5);
	exit(0);
}