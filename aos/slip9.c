// Q1

#include <stdio.h>
#include <stdlib.h>
int main()
{
	printf("Old Environment variables\n");

	printf("PATH : %s\n", getenv("PATH"));
	printf("HOME : %s\n", getenv("HOME"));
	printf("ROOT : %s\n", getenv("ROOT"));
	setenv("PATH", "", 1);
	setenv("HOME", "", 1);
	setenv("ROOT", "", 1);
	printf("New Environment variables\n");
	printf("PATH : %s\n", getenv("PATH"));
	printf("HOME : %s\n", getenv("HOME"));
	printf("ROOT : %s\n", getenv("ROOT"));
	return (0);
}

// Q2

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
void insertionSort(char *arr[], int n)
{
	int i, j;
	char *key;
	for (i = 1; i < n; i++)
	{
		key = arr[i];

		j = i - 1;
		while (j >= 0 && strcmp(arr[j], key) > 0)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}
void main()
{
	struct dirent *de;
	DIR *directory = opendir(".");
	char **filenames;
	if (directory == NULL)
	{
		printf("Could not open current directory");
		return;
	}
	char **subdirectories;
	int dirCount = 0;
	while ((de = readdir(directory)) != NULL)
		if (strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && de->d_type == DT_DIR)
		{
			dirCount++;
		}
	subdirectories = (char **)malloc(sizeof(char *) * dirCount);
	rewinddir(directory);
	int i = 0;
	while ((de = readdir(directory)) != NULL)
		if (strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && de->d_type == DT_DIR)
		{
			subdirectories[i] = de->d_name;
			i++;
		}
	closedir(directory);
	insertionSort(subdirectories, dirCount);
	for (i = 0; i < dirCount; i++)
	{
		printf("%s\n", subdirectories[i]);
	}
}