#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


long int revert_read(int fd, char *buf, size_t f_size)
{
    long int rrbytes = 0, psymbol = 0;
    char rbuf;

    for (long int i = f_size; i >= 0; i--)
    {
        lseek(fd, i, SEEK_SET);
        rrbytes += read(fd, &rbuf, 1);
        buf[psymbol] = rbuf;
        psymbol += 1;
    }
    return rrbytes;
}


int main(int argc, char *argv[])
{
    int fd;
    long int rbytes, f_size;
    char buffer[512];

    if (argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd > 0)
        {
            f_size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            rbytes = revert_read(fd, buffer, f_size - 2);
            if (rbytes > 0)
            {
                printf("Revert readed data:\n%s\n", buffer);
            }
            else
                perror("Error while readed data");
        }
        else
            perror("Error while opening file");
    }
    else
        perror("Don't provide filename");
    return 0;
}
