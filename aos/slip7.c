// Q1

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
void main()
{
	int fd = open("hole.txt", O_CREAT | O_RDWR);
	system("chmod 722 hole.txt");
	char message[] = "This is a demonstration";
	char message2[] = " for hole in a file.";
	write(fd, message, sizeof(message));   // write first part
	lseek(fd, 10, SEEK_END);			   // adding hole of 10 characters
	write(fd, message2, sizeof(message2)); // write second part
	system("od -c hole.txt");
}

// Q2
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void print_permissions(struct stat filestat)
{
	printf(S_ISDIR(filestat.st_mode) ? "d" : "-");
	printf(filestat.st_mode & S_IRUSR ? "r" : "-");
	printf(filestat.st_mode & S_IWUSR ? "w" : "-");

	printf(filestat.st_mode & S_IXUSR ? "x" : "-");
	printf(filestat.st_mode & S_IRGRP ? "r" : "-");
	printf(filestat.st_mode & S_IWGRP ? "w" : "-");
	printf(filestat.st_mode & S_IXGRP ? "x" : "-");
	printf(filestat.st_mode & S_IROTH ? "r" : "-");
	printf(filestat.st_mode & S_IWOTH ? "w" : "-");
	printf(filestat.st_mode & S_IXOTH ? "x" : "-");
}
void main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("No file name provided");
		exit(1);
	}
	char *filename = argv[1];
	struct stat filestat;
	int ret = stat(filename, &filestat);
	if (ret < 0)
	{
		printf("Error getting file info.\n");
	}
	struct passwd *pw = getpwuid(filestat.st_uid);
	struct group *gw = getgrgid(filestat.st_gid);
	struct tm *modified_time = localtime(&filestat.st_mtime);
	time_t ctime = time(NULL);
	struct tm *current_time = localtime(&ctime);
	print_permissions(filestat);

	printf(" %ld %s %s %ld ", filestat.st_nlink, pw->pw_name, gw->gr_name,
		   filestat.st_size);
	char timestr[80];
	if (modified_time->tm_year == current_time->tm_year)
	{
		strftime(timestr, 80, "%b %d %l:%M %p", modified_time);
		printf("%s ", timestr);
	}
	else
	{
		strftime(timestr, 80, "%b %d", modified_time);
		printf("%s %d ", timestr, modified_time->tm_year);
	}
	printf("%s\n", filename);
}