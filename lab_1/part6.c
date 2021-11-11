
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int fd, wbytes, rbytes, hbytes, f_size, ret;
    char msg[] = "Hello, world!\nPython is the best programming language!\n";
    char buffer[512];
    char* filename;
    struct stat file_stat;

    filename = argv[1];

    fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0600); // Alias creat()
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

                lseek(fd, -10, SEEK_END);
                rbytes = read(fd, buffer, sizeof(buffer));
                printf("SEEK_END - 10:\n%s", buffer);

                lseek(fd, 13, SEEK_SET);
                rbytes = read(fd, buffer, sizeof(buffer));
                printf("SEEK_SET + 13:\n%s", buffer);

                // Sparse file
                memset(buffer, 0, sizeof(buffer));
                f_size = lseek(fd, 0, SEEK_END);
                printf("File size in bytes: %d\n", f_size);
                lseek(fd, 10*1024, SEEK_SET);
                write(fd, msg, sizeof(msg) - 1);

                ret = stat(filename, &file_stat);
                if (ret == 0)
                {
                    printf("Sparse file size in bytes: %lld\n", file_stat.st_size);
                    printf("System block size in bytes: %d\n", file_stat.st_blksize);
                    printf("Sparse file size in blocks: %lld\n", file_stat.st_blocks);
                }
                else
                {
                    perror("Can't get file statistic");
                }
                close(fd);
            }
        }
        else
        {
            perror("Write error");
        }
    }
    else
    {
        perror("Create error");
    }
    return 0;
}