// Q1
#include <stdio.h>
#include <alloca.h>
void main()
{
	int i, n;
	int *a;
	printf("Enter the number of elements : ");
	scanf("%d", &n);
	a = (int *)alloca(n * sizeof(int));
	printf("Enter %d elements...\n", n);
	for (i = 0; i < n; i++)
	{
		printf("%d : ", i);
		scanf("%d", &a[i]);
	}
	printf("You entered...\n");
	for (i = 0; i < n; i++)
	{
		printf("%d\t", a[i]);
	}
	printf("\n");
}

// Q2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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
void count_process(char **arguments, int argc)
{
	if (argc != 3)
	{
		printf("Invalid arguments\n");
		exit(1);
	}
	FILE *fp = fopen(arguments[2], "r");
	if (strcmp(arguments[1], "c") == 0)
	{
		int charactercount = 0;
		while (fgetc(fp) != EOF)
			charactercount++;
		printf("Characters : %d\n", charactercount);
	}
	else if (strcmp(arguments[1], "w") == 0)
	{
		int wordcount = 0;
		char c;
		while ((c = fgetc(fp)) != EOF)
		{
			if (c == ' ')
				wordcount++;
		}
		printf("Words : %d\n", wordcount);
	}
	else if (strcmp(arguments[1], "l") == 0)
	{
		int linecount = 0;
		char c;
		while ((c = fgetc(fp)) != EOF)
		{
			if (c == '\n')
				linecount++;
		}
		printf("Lines : %d\n", linecount);
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
			if (strcmp(arguments[0], "count") == 0)
			{
				count_process(arguments, argc);
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