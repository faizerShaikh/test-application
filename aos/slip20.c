// Q1

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
void child_process()
{
	printf("Hello World\n");
}
void main()
{
	int pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "Error in fork execution\n");
		exit(1);
	}
	if (pid == 0)
	{
		child_process();
		exit(0);
	}
	int status;
	while (wait(&status) > 0)
		;
	printf("Child exited with status : %d\n", WEXITSTATUS(status));
}

// Q2
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
void main()
{
	DIR *dir = opendir(".");
	struct dirent *dent;
	mkdir("./backup", 0777);
	char tofilename[256];
	while ((dent = readdir(dir)) != NULL)
	{
		if (dent->d_type == DT_REG)
		{
			printf("copying file : %s\n", dent->d_name);
			if (fork() == 0)
			{
				strcat(strcat(tofilename, "./backup/"), dent->d_name);
				printf("to %s\n", tofilename);
				char *args[] = {"cp", dent->d_name, tofilename, NULL};
				execvp("cp", args);
				exit(0);
			}
		}
	}
	int status;
	while (wait(&status) > 0)
		;
}