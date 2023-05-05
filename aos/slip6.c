// Q1

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define BUFFER_SIZE 100
int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Please pass file name\n");
		return 1;
	}
	char *addr;
	char buffer[BUFFER_SIZE];
	int fd = open(argv[1], O_RDONLY);
	struct stat st;
	if (fstat(fd, &st) < 0)
	{
		fprintf(stderr, "Error reading file info\n");
		return 1;
	}
	if ((addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "Error mapping file\n");
		return 1;
	}
	for (int i = st.st_size; i >= 0; i--)
	{
		printf("%c", addr[i]);
	}
	munmap(addr);
	close(fd);
	return 0;
}

// Q2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
int occurrence(char *str, char ch)
{
	int count = 0;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		if (ch == str[i])
			++count;
	}
	return count;
}
void list_process(char **arguments, int argc)
{
	if (argc != 3)
	{
		printf("Invalid arguments\n");
		exit(1);
	}

	DIR *directory = opendir(arguments[2]);
	struct dirent *de;
	char **filenames;
	if (directory == NULL)
	{
		printf("Could not open current directory");
		return;
	}
	if (strcmp(arguments[1], "f") == 0)
	{
		while ((de = readdir(directory)) != NULL)
			if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
			{
				printf("%s\n", de->d_name);
			}
	}
	else if (strcmp(arguments[1], "n") == 0)
	{
		int fileCount = 0;
		while ((de = readdir(directory)) != NULL)
			if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
			{
				fileCount++;
			}
		printf("Total files : %d\n", fileCount);
	}
	else if (strcmp(arguments[1], "i") == 0)
	{
		while ((de = readdir(directory)) != NULL)
			if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
			{
				printf("%s -> %ld\n", de->d_name, de->d_ino);
			}
	}
	closedir(directory);
}
void main()
{
	char *cmd = (char *)malloc(100 * sizeof(char));
	char *delimeter = " ";
	int status;
	char username[20];
	getlogin_r(username, 20);
	while (1)
	{
		printf("%s$ ", username);
		fgets(cmd, 100, stdin);
		if (cmd[strlen(cmd) - 1] == '\n')
		{
			cmd[strlen(cmd) - 1] = '\0';
		}
		if (cmd[strlen(cmd) - 1] == ' ')
		{
			cmd[strlen(cmd) - 1] = '\0';
		}
		int occur = occurrence(cmd, ' ');
		int argc = occur + 1;
		char **arguments = (char **)malloc((argc + 1) * sizeof(char *));
		int i = 0;
		char *token = strtok(cmd, delimeter);
		int len = strlen(token);
		arguments[i] = malloc(len);

		strcpy(arguments[i], token);
		i++;
		while (i <= occur)
		{
			token = strtok(NULL, delimeter);
			int len = strlen(token);
			arguments[i] = malloc(len);
			strcpy(arguments[i], token);
			i++;
		}
		if (fork() == 0)
		{
			if (strcmp(arguments[0], "list") == 0)
			{
				list_process(arguments, argc);
				exit(0);
			}
			else
			{
				execvp(arguments[0], arguments);
			}
		}
		while (wait(&status) > 0)
			;
		free(arguments);
	}
}