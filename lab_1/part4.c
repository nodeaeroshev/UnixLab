#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int fd, rbytes, wbytes, hbytes;
    char msg[] = "Hello, world!\nPython is the best programming language!\n";
    char buffer[512];
    char* filename;

    filename = argv[1];

    fd = creat(filename, 0600);
    if (fd != -1)
    {
        wbytes = write(fd, msg, sizeof(msg) - 1);
        if (wbytes != -1)
        {
            hbytes = wbytes / 2;
            fd = open(filename, O_RDWR);
            if (fd != -1)
            {
                rbytes = read(fd, buffer, wbytes);
                printf("Readed:\n%s", buffer);
                memset(buffer, 0, sizeof(buffer));

                rbytes = pread(fd, buffer, hbytes + 1, hbytes);
                printf("Read half data:\n%s\n", buffer);
                memset(buffer, 0, sizeof(buffer));

                // TODO
                lseek(fd, 0, SEEK_END);
                rbytes = read(fd, buffer, hbytes);
                printf("SEEK_END + 0:\n%s", buffer);

                // TODO
                lseek(fd, -10, SEEK_SET);
                rbytes = read(fd, buffer, hbytes);
                printf("SEEK_SET - 10:\n%s", buffer);

                close(fd);
            }
        }
        else
        {
            perror("Write failed!");
        }
    }
    else
    {
        perror("Create failed!");
    }
    return 0;
}