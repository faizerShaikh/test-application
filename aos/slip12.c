// Q1

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
void catch (int);
int main(void)
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "Error in fork execution\n");
		return 1;
	}

	if (pid != 0)
	{
		signal(SIGALRM, catch);
		pause();
	}
	else
	{
		kill(getppid(), SIGALRM);
	}
	return 0;
}
void catch (int signo)
{
	write(STDOUT_FILENO, "alarm is fired\n", 15);
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
		printf("no size value passed\n");
		exit(1);
	}
	int size = atoi(argv[1]);

	if (size < 0)
	{
		printf("invalid size value : size should be non negative\n");
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
			if (fstat.st_size > size)
			{
				printf("%s\n", de->d_name);
			}
		}
	closedir(directory);
}