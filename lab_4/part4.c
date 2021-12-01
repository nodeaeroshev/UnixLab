#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>


enum PipeAction
{
    READ,
    WRITE
};


int main()
{
    pid_t pid;
    int fd_ctp[2];
    char buf[16];
    char ptr_fd;
    int tmp;

    int ok_ctp = pipe(fd_ctp);

    if (!ok_ctp)
    {
        pid = fork();

        switch(pid)
        {
            case -1:
                perror("Fork failed!");
                exit(1);
            case 0:
                close(fd_ctp[READ]);
                if ((tmp = dup(fd_ctp[WRITE])) != -1)
                {
                    ptr_fd = tmp + '0';
                    execl("/Users/artem/UnixLab/lab_4/part4_co.o", &ptr_fd, NULL);
                }
                exit(0);
            default:
                fprintf(stdout, "PARENT process - %d\n", getpid());
                close(fd_ctp[WRITE]);

                while (read(fd_ctp[READ], buf, sizeof(buf)) > 0)
                {
                    fprintf(stdout, "Readed: %s\n", buf);
                    memset(buf, 0, sizeof(buf));
                }
                
                close(fd_ctp[READ]);
                fprintf(stdout, "SHUTDOWN PARENT\n");

        }
    }
    else
        return 1;

    return 0;
}
