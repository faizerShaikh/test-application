// Q1

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void main()
{
	int fd;
	fd = open("output.txt", O_CREAT | O_WRONLY, 0777);
	close(STDOUT_FILENO);
	dup(fd);
	printf("this is some text to be printed on the screen\n");

	printf("but it will be written to the file output.txt\n");
}

// Q2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#define max(x, y) ((x) > (y)) ? (x) : (y);
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
void print_first_lines(char *linesString, FILE *fp)
{
	char c;
	int lines = atoi(linesString);
	int lineCount = 1;
	printf("%3d |", lineCount);
	while ((c = fgetc(fp)) != EOF)
	{
		fputc(c, stdout);
		if (c == '\n')
		{
			lineCount++;
			if (lineCount > lines)
			{
				break;
			}
			printf("%3d |", lineCount);
		}
	}
	printf("\n");
}
void print_last_lines(char *linesString, FILE *fp)
{
	char c;
	int lines = atoi(linesString);
	int lineCount = 0;
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			lineCount++;
	}
	int startLine = max(0, lineCount + lines);
	lineCount = 0;
	fseek(fp, 0, SEEK_SET);
	while ((c = fgetc(fp)) != EOF)
	{
		if (lineCount > startLine)
		{
			fputc(c, stdout);
		}
		if (c == '\n')
		{
			lineCount++;
			if (lineCount > startLine)
			{
				printf("%3d |", lineCount);
			}
		}
	}
	printf("\n");
}
void print_all_lines(FILE *fp)
{
	char c;
	while ((c = fgetc(fp)) != EOF)
	{
		fputc(c, stdout);
	}
	printf("\n");
}
void typeline_process(char **arguments, int argc)
{
	if (argc != 3)
	{
		printf("Invalid arguments\n");
		exit(1);
	}
	FILE *fp = fopen(arguments[2], "r");
	if (arguments[1][0] == '+')
	{
		print_first_lines(arguments[1], fp);
	}
	else if (arguments[1][0] == '-')
	{
		print_last_lines(arguments[1], fp);
	}
	else if (strcmp(arguments[1], "a") == 0)
	{
		print_all_lines(fp);
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
			if (strcmp(arguments[0], "typeline") == 0)
			{
				typeline_process(arguments, argc);
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