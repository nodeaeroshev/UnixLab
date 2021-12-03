#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>


int main()
{
    pid_t pid;
    int p_rbytes, c_rbytes;
    char p_buf[512], c_buf[512];

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Failed fork");
            exit(1);
        case 0:
            while(1)
            {
                c_rbytes = read(0, c_buf, sizeof(c_buf));
                fprintf(stdout, "CHILD: Readed %d bytes in message -> %s\n", c_rbytes, c_buf);
            }
            exit(0);
        default:
            while(1)
            {
                p_rbytes = read(0, p_buf, sizeof(p_buf));
                fprintf(stdout, "PARENT: Readed %d bytes in message -> %s\n", p_rbytes, p_buf);
            }
            wait(NULL);
            exit(0);
    }

    return 0;
}
