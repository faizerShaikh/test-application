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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#define max(x, y) ((x) > (y)) ? (x) : (y);
#define RESET "\033[0m"
#define GREEN "\033[32m"
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
int search(char *pattern, char *text, int lineNo, int first, int count)
{
	int M = strlen(pattern);
	int N = strlen(text);
	int found = 0, i, j;
	/* A loop to slide pat[] one by one */
	for (i = 0; i <= N - M; i++)
	{
		for (j = 0; j < M; j++)
			if (text[i + j] != pattern[j])
				break;
		if (j == M)
		{
			found++;
		}
	}
	if (found && !count)
	{
		int printed = 0;
		printf("%3d |", lineNo + 1);
		for (i = 0; i < N; i++)
		{
			if (i < N - M)
			{
				for (j = 0; j < M; j++)
					if (text[i + j] != pattern[j])
						break;
			}
			if (j == M && i < N - M && !(first && printed))
			{
				printf("%s%s%s", GREEN, pattern, RESET);
				printed++;
				i += (M - 1);
			}
			else
			{
				printf("%c", text[i]);
			}
		}
	}
	return found;
}
int count_lines(FILE *fp)
{
	int c;
	int lines = 0;
	while ((c = fgetc(fp)))
	{
		if (c == '\n')
		{
			lines++;
		}
		if (feof(fp))
		{
			break;
		}
	}
	fseek(fp, 0, SEEK_SET);
	return lines;
}
void print_occurrence(char *pattern, FILE *fp, int first, int count)
{
	int lines = count_lines(fp);
	int l, found;
	char **fileContent = (char **)malloc(lines * sizeof(char *));
	// allocate memory for each of those lines according to its character count
	for (l = 0; l < lines; l++)
	{
		fileContent[l] = (char *)malloc(1000 * sizeof(char));
	}
	for (l = 0; l < lines; l++)
	{
		fgets(fileContent[l], 1000, fp);
	}
	int totalCount = 0;
	for (l = 0; l < lines; l++)
	{
		if (found = search(pattern, fileContent[l], l, first, count))
		{
			if (first)
			{
				return;
			}
			totalCount += found;
		}
	}
	if (count)
	{
		printf("Total Occurrences : %d\n", totalCount);
	}
	for (l = 0; l < lines; l++)
	{
		free(fileContent[l]);
	}
	free(fileContent);
}
void search_process(char **arguments, int argc)
{
	if (argc != 4)
	{
		printf("Invalid arguments\n");
		return;
	}
	FILE *fp = fopen(arguments[3], "r");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		return;
	}
	char *pattern = arguments[2];
	if (strcmp(arguments[1], "f") == 0)
	{
		print_occurrence(pattern, fp, 1, 0);
	}
	else if (strcmp(arguments[1], "c") == 0)
	{
		print_occurrence(pattern, fp, 0, 1);
	}
	else if (strcmp(arguments[1], "a") == 0)
	{
		print_occurrence(pattern, fp, 0, 0);
	}
	fclose(fp);
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
			if (strcmp(arguments[0], "search") == 0)
			{
				search_process(arguments, argc);
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