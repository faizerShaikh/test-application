// Q1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MESSAGE_BLOCK_SIZE 20
void main()
{
	int fd[2];
	char message1[MESSAGE_BLOCK_SIZE] = "Hello World";
	char message2[MESSAGE_BLOCK_SIZE] = "Hello SPPU";
	char message3[MESSAGE_BLOCK_SIZE] = "Linux is Funny";

	char message[MESSAGE_BLOCK_SIZE];
	if (pipe(fd) < 0)
	{
		printf("Error creating pipe\n");
		exit(1);
	}
	int pid = fork();
	if (pid == 0)
	{
		write(fd[1], message1, MESSAGE_BLOCK_SIZE);
		write(fd[1], message2, MESSAGE_BLOCK_SIZE);
		write(fd[1], message3, MESSAGE_BLOCK_SIZE);
		exit(0);
	}
	int status;
	while (wait(&status) > 0)
		;
	read(fd[0], message, MESSAGE_BLOCK_SIZE);
	printf("%s\n", message);
	read(fd[0], message, MESSAGE_BLOCK_SIZE);
	printf("%s\n", message);
	read(fd[0], message, MESSAGE_BLOCK_SIZE);
	printf("%s\n", message);
}

// Q2

#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int doesEndWithTxt(char *string)
{
	char *substr = ".txt";
	int length = 4;
	int lengthLong = strlen(string) - 1;
	int doesEnd = 1;
	while (length--)
	{
		if (string[lengthLong] != substr[length])
		{
			doesEnd = 0;
			break;
		}
		lengthLong--;
	}
	return doesEnd;
}
void main(int argc, char *argv[])
{
	struct dirent *de;
	DIR *directory = opendir(".");
	char **filenames;
	if (directory == NULL)
	{

		printf("Could not open current directory");
		return;
	}
	int mergedfd = open("merged.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	char block[8];
	while ((de = readdir(directory)) != NULL)
		if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, ".."))
		{
			if (strlen(de->d_name) > 4 && doesEndWithTxt(de->d_name))
			{
				printf("%s\n", de->d_name);
				int fd = open(de->d_name, O_RDONLY);
				while (read(fd, block, 8) > 0)
				{
					write(mergedfd, block, 8);
				}
				close(fd);
			}
		}
	printf("Merged file : merged.txt\n");
	printf("file descriptor : %d\n", mergedfd);
	close(mergedfd);
	closedir(directory);
}