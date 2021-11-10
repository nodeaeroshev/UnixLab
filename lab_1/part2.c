#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int fd, rbytes, wbytes;
    char msg[] = "Hello, world!\nPython is the best programming language!\n";
    char* filename;
    char buffer[512];
    unsigned int access_mode = strtol(argv[2], NULL, 8);
    filename = argv[1];

    // Создание
    fd = creat(filename, access_mode);
    if (fd != -1)
    {
        printf("Create successfully: %s\n", filename);
        // Запись
        wbytes = write(fd, msg, sizeof(msg) - 1);
        if (wbytes != -1)
        {
            printf("Successfully write in file:\n%s", msg);
            // Открываем уже на чтение
            fd = open(filename, O_RDONLY);
            if (fd != -1)
            {
                rbytes = read(fd, buffer, sizeof(buffer));
                printf("Readed data:\n%s", buffer);
            }
            else
            {
                printf("Open failed: %s\n", strerror(errno));
            }
            if (close(fd) != -1)
            {
                fd = open(filename, O_RDWR);
                if (fd != -1)
                {
                    printf("Successfully opened in mode O_RDWR!\n");
                }
                else
                {
                    printf("Open failed: %s\n", strerror(errno));
                }
                close(fd);
            }
            else
            {
                printf("Close failed: %s\n", strerror(errno));  
            }
        }
        else
        {
            printf("%d Write failed: %s\n", wbytes, strerror(errno));
        }

    }
    else
    {
        perror("Creation failed");
    }
    return 0;
}