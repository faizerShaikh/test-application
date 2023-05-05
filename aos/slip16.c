// Q1

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void sigint()
{
	write(STDOUT_FILENO, "Press Ctrl + C once again to exit", 33);
	signal(SIGINT, SIG_DFL);
}
void main()
{
	signal(SIGINT, sigint);
	while (1)
		;
}

// Q2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
void main()
{
	int fd = open("output.txt", O_CREAT | O_RDWR);
	if (fork() == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			printf("Invalid file discriptor\n");
		}
		char *args[] = {"ls", "-l", NULL};
		int ret = execvp("ls", args);
		if (ret < 0)
		{

			printf("Program can't be executed\n");
		}
	}
	int status;
	while (wait(&status) > 0)
		;
}