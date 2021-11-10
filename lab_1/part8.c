#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define CHUNK_SIZE 512


int copy_file(int fd_source, int fd_target)
{
    // TODO
    long int rbytes = 0, wbtytes = 0;
    long int readed_b, f_size;
    int chunks, tail;
    char buffer[CHUNK_SIZE];

    f_size = lseek(fd_source, 0, SEEK_END);
    chunks = f_size / CHUNK_SIZE;
    tail = f_size % CHUNK_SIZE;
    printf("%d\n", chunks);
    printf("%d\n", tail);

    for (int chunk = 0; chunk < chunks; chunk++)
    {
        readed_b = read(fd_source, buffer, CHUNK_SIZE);
        rbytes += readed_b;
        wbtytes += write(fd_target, buffer, CHUNK_SIZE);
    }
    if (tail > 0)
    {
        readed_b = read(fd_source, buffer, tail);
        printf("%d\n", readed_b);
        rbytes += readed_b;
        printf("%d\n", rbytes);
        wbtytes += write(fd_target, buffer, tail);
        printf("%d\n", wbtytes);
    }

    if (wbtytes != rbytes)
        return 1;

	return 0;
}


int main(int argc, char *argv[])
{
	int fd_source = 0, fd_target = 0, status = 1;

	if (argc > 1)
	{
		fd_source = open(argv[1], O_RDONLY);
		if (fd_source == -1)
			printf("Error while opening source file!\n");
		fd_target = creat(argv[2], 0600);
		if (fd_target == -1)
			printf("Error while creating target file!\n");
	}
	if (fd_source > 0 && fd_target > 0)
	{
		status = copy_file(fd_source, fd_target);
		if (!status)
			printf("Sucessfuly copy file!\n");
		else
			printf("Error while copy file!\n");
		close(fd_source);
		close(fd_target);
	}
	return status;
}