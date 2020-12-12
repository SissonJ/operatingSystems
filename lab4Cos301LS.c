#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct dirent *thisDir;
	char *buf = ".";
	struct DIR *wd;	
	if(argc == 1)
	{
		wd = (struct DIR *) opendir(buf);
		while((thisDir = (struct dirent *) readdir(wd))!= NULL)
		{
			printf("%s\n", thisDir->d_name);
		}		
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "-l") == 0)
		{
			wd = (struct DIR *) opendir(".");
			while((thisDir = (struct dirent *) readdir(wd)) != NULL)
			{
				printStat(&thisDir->d_name);
			}
		}
		else if(opendir(argv[1]) != NULL)
		{
			wd = opendir(argv[1]);
			while((thisDir = readdir(wd)) != NULL)
			{
				printf("%s\n", thisDir->d_name);
			}
		}
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1],"-l")==0 && opendir(argv[2]) != NULL)
		{
			wd = opendir(argv[2]);
			while((thisDir = readdir(wd)) != NULL)
			{
				printStat(&thisDir->d_name);
			}
		}
		else if(strcmp(argv[2], "-l")==0 && opendir(argv[1]) != NULL)
		{
			wd = opendir(argv[1]);
			while((thisDir = readdir(wd))!= NULL)
			{
				printStat(&thisDir->d_name);
			}
		}
	}
	else
	{
		printf("Please enter valid arguments");
	}
	return 0;
}

void printStat(char *fileName)
{
	struct stat sb;
	char *time;
	if(stat(fileName, &sb)!=-1)
	{
		time = ctime(&sb.st_atime);
		printf("%lo %ld %ld %ld %lld %s %s\n", 
			(unsigned long) sb.st_mode,
			(long) sb.st_nlink,
			(long) sb.st_uid,
			(long) sb.st_gid,
			(long long) sb.st_size,
			strtok(time,"\n"),
			fileName);	
	}
}