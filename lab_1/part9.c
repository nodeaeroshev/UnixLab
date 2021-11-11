#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define CHUNK_SIZE 10


int copy_file(int fd_source, int fd_target)
{
    long int rbytes = 0, wbtytes = 0;
    long int readed_b, f_size;
    int chunks, tail;
    char buffer[CHUNK_SIZE];

    f_size = lseek(fd_source, 0, SEEK_END);
    lseek(fd_source, 0, SEEK_SET);

    chunks = f_size / CHUNK_SIZE;
    tail = f_size % CHUNK_SIZE;

    for (int chunk = 0; chunk < chunks; chunk++)
    {
        readed_b = read(fd_source, buffer, CHUNK_SIZE);
        rbytes += readed_b;
        wbtytes += write(fd_target, buffer, CHUNK_SIZE);
    }
    if (tail > 0)
    {
        readed_b = read(fd_source, buffer, tail);
        rbytes += readed_b;
        wbtytes += write(fd_target, buffer, tail);
    }

    if (wbtytes != rbytes)
        return 1;

    fsync(fd_target);

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
        if (dup2(fd_source, 0) == -1)
        {
            perror("Error retrasnlate to stdin!");
            return status;
        }
        if (dup2(fd_target, 1) == -1)
        {
            perror("Error retrasnlate to stdout!");
            return status;
        }
        close(fd_source);
        close(fd_target);
    }

    // TODO
    status = copy_file(0, 1);
    if (!status)
        printf("Sucessfuly copy file!\n");
    else
        printf("Error while copy file!\n");

    return status;
}
