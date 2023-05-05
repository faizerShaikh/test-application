// Q1

#include <stdio.h>
#include <stdlib.h>
void main()
{
	FILE *fp1, *fp2;
	char ch;
	fp1 = fopen("file1.txt", "r");
	fp2 = fopen("file2.txt", "w");
	while ((ch = fgetc(fp1)) != EOF)
	{
		putc(ch, fp2);
	}
	printf("File copied Successfully!");
	fclose(fp1);
	fclose(fp2);
	remove("file1.txt");
}

// Q2

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
int signalReceived = 0;
int sigid = 0;
void child_process(char **arguments)
{
	if ((execvp(arguments[0], arguments)) < 0)
	{
		fprintf(stderr, "error running command : %s\n", arguments[0]);
		exit(1);
	}
}

void sigalarm(int sig)
{
	signalReceived = 1;
	sigid = SIGALRM;
}
void sigchild(int sig)
{
	signalReceived = 1;
	sigid = SIGCHLD;
}
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage : %s <command> [arguments]\n", argv[0]);
		return 1;
	}
	int pid = fork();
	int alarmCounter = 0;
	if (pid < 0)
	{
		fprintf(stderr, "Error in fork execution\n");
		return 1;
	}
	if (pid == 0)
	{
		child_process(&argv[1]);
		exit(0);
	}
	signal(SIGCHLD, sigchild);
	signal(SIGALRM, sigalarm);
	alarm(1);
	while (1)
	{
		if (signalReceived)
		{
			signalReceived = 0;
			if (sigid == SIGALRM)
			{

				alarmCounter++;
				if (alarmCounter < 5)
				{
					alarm(1);
					continue;
				}
			}
			if (alarmCounter >= 5)
			{
				kill(pid, SIGKILL);
				break;
			}
			if (sigid == SIGCHLD)
			{
				signal(SIGALRM, SIG_DFL);
				return 0;
			}
		}
	}
	return 0;
}