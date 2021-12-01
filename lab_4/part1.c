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
    int fd_ptc[2], fd_ctp[2];
    char msg[] = "Hello, world!\n";
    char answ[] = "Hello, Linux!\n";
    char buf[512];

    int ok_ptc = pipe(fd_ptc);
    int ok_ctp = pipe(fd_ctp);

    if (!(ok_ptc & ok_ctp))
    {
        pid = fork();

        switch(pid)
        {
            case -1:
                perror("Fork failed!");
                exit(1);
            case 0:
                fprintf(stdout, "CHILD process - %d\n", getpid());
                close(fd_ptc[WRITE]);
                close(fd_ctp[READ]);

                memset(buf, 0, sizeof(buf));
                read(fd_ptc[READ], buf, sizeof(buf));
                fprintf(stdout, "Readed: %s\n", buf);

                write(fd_ctp[WRITE], answ, sizeof(answ));
                fprintf(stdout, "Wrote: %s\n", answ);

                close(fd_ptc[READ]);
                close(fd_ctp[WRITE]);
                fprintf(stdout, "SHUTDOWN CHILD\n");
                exit(0);
            default:
                fprintf(stdout, "PARENT process - %d\n", getpid());
                close(fd_ptc[READ]);
                close(fd_ctp[WRITE]);

                write(fd_ptc[WRITE], msg, sizeof(msg));
                fprintf(stdout, "Wrote: %s\n", msg);

                memset(buf, 0, sizeof(buf));
                read(fd_ctp[READ], buf, sizeof(buf));
                fprintf(stdout, "Readed: %s\n", buf);

                close(fd_ptc[WRITE]);
                close(fd_ctp[READ]);
                fprintf(stdout, "SHUTDOWN PARENT\n");

        }
    }
    else
        return 1;

    return 0;
}
