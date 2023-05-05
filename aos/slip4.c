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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
void sigint()
{
}
void main()
{
	int filedes[2];
	if (pipe(filedes) == -1)
	{
		perror("pipe");
		exit(1);
	}
	signal(SIGINT, sigint);
	if (fork() == 0)
	{
		while ((dup2(filedes[1], STDOUT_FILENO) == -1))
		{
		}
		char *args[] = {"ls", "-l", NULL};
		int ret = execvp("ls", args);
		if (ret < 0)
		{
			printf("Program can't be executed\n");
		}
		exit(0);
	}
	close(filedes[1]);
	if (fork() == 0)
	{
		while ((dup2(filedes[0], STDIN_FILENO) == -1))
		{
		}
		char *args[] = {"wc", "-l", NULL};

		int ret = execvp("wc", args);
		if (ret < 0)
		{
			printf("Program can't be executed\n");
		}
		exit(0);
	}
	char output[100];
	read(filedes[0], output, 100);
	printf("%s", output);
	close(filedes[0]);
	// exit(0);
	int i = 0;
	while (i < 2000000000)
		i++;
}