// Q1

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage : %s <filepath>\n", argv[0]);
		return 1;
	}
	int file = open(argv[1], O_RDONLY);
	if (file < 0)
	{
		fprintf(stderr, "error opening file\n");
		return 1;
	}
	struct stat st;
	if (fstat(file, &st) < 0)
	{
		fprintf(stderr, "error reading file info\n");
		return 1;
	}
	printf("%s Details : \n", argv[1]);
	printf("File size : %ld\n", st.st_size);
	printf("Number of hard links : %ld\n", st.st_nlink);
	printf("File inode : %ld\n", st.st_ino);
	printf("File Permissions : ");
	printf(S_ISDIR(st.st_mode) ? "d" : "-");

	printf((st.st_mode & S_IRUSR) ? "r" : "-");
	printf((st.st_mode & S_IWUSR) ? "w" : "-");
	printf((st.st_mode & S_IXUSR) ? "x" : "-");
	printf((st.st_mode & S_IRGRP) ? "r" : "-");
	printf((st.st_mode & S_IWGRP) ? "w" : "-");
	printf((st.st_mode & S_IXGRP) ? "x" : "-");
	printf((st.st_mode & S_IROTH) ? "r" : "-");
	printf((st.st_mode & S_IWOTH) ? "w" : "-");
	printf((st.st_mode & S_IXOTH) ? "x" : "-");
	printf("\n");
	char timestr[50];
	struct tm *modified_time = localtime(&st.st_mtime);
	strftime(timestr, 80, "%b %d %l:%M %p", modified_time);
	printf("Modified time : %s\n", timestr);
	struct tm *access_time = localtime(&st.st_atime);
	strftime(timestr, 80, "%b %d %l:%M %p", access_time);
	printf("Access time : %s\n", timestr);
	return 0;
}

// Q2

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
volatile int sigid = 0;
// function declaration
void sighup(int sig);
void sigint(int sig);
void sigquit(int sig);
// driver code

void main()
{
	int pid, timeCounter;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{ /* child */
		signal(SIGHUP, sighup);
		signal(SIGINT, sigint);
		signal(SIGQUIT, sigquit);
		while (1)
		{ /* loop for ever */
			if (sigid == SIGHUP)
			{
				printf("CHILD: I have received a SIGHUP\n");
				sigid = 0;
				continue;
			}
			else if (sigid == SIGINT)
			{
				printf("CHILD: I have received a SIGINT\n");
				sigid = 0;
				continue;
			}
			else if (sigid == SIGQUIT)
			{
				printf("My Daddy has killed me!!!\n");
				sigid = 0;
				break;
			}
		}
		exit(0);
	}
	sleep(1);
	fflush(stdout);

	while (1)
	{
		printf("\nPARENT: sending SIGHUP to process : %d\n\n", pid);
		kill(pid, SIGHUP);
		timeCounter += 3;
		if (timeCounter > 15)
		{
			break;
		}
		sleep(3); /* pause for 3 secs */
		printf("\nPARENT: sending SIGINT to process : %d\n\n", pid);
		kill(pid, SIGINT);
		timeCounter += 3;
		if (timeCounter > 15)
		{
			break;
		}
		sleep(3); /* pause for 3 secs */
	}
	printf("\nPARENT: sending SIGQUIT to process : %d\n\n", pid);
	usleep(10000);
	kill(pid, SIGQUIT);
	sleep(1);
}
void sighup(int sig)
{
	sigid = SIGHUP;
}
void sigint(int sig)
{
	sigid = SIGINT;
}
void sigquit(int sig)
{

	sigid = SIGQUIT;
}