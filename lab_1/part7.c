#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[])
{
	int ret;
	struct stat file_stat;
	char *filename;

	filename = argv[1];

	ret = stat(filename, &file_stat);
	if (ret == 0)
	{
        printf("File permissions: %o\n", file_stat.st_mode & 0777);
        printf("Number of hard links: %d\n", file_stat.st_nlink);
        printf("File size in bytes: %lld\n", file_stat.st_size);
        printf("File size in blocks: %lld\n", file_stat.st_blocks);
        printf("User ID: %d\n", file_stat.st_uid);
        printf("Group ID: %d\n", file_stat.st_gid);
        printf("Last access time:\t\t\t%s\n", asctime(localtime(&file_stat.st_atime)));
        printf("Last content modfication time:\t\t%s\n", asctime(localtime(&file_stat.st_mtime)));
        printf("Last attributes modfication time:\t%s\n", asctime(localtime(&file_stat.st_ctime)));
	}
	else
	{
        printf("Error get statistics");
        return 1;
	}
	return 0;
}