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
    int fd_ctp[2], fd_ptc[2];
    char msg[] = "Hello, world!\n";
    char answ[] = "Hello, Linux!\n";
    char c_buf[16], p_buf[16];

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
                close(fd_ctp[READ]);
                close(fd_ptc[WRITE]);

                for (int i = 0; i < 10; i++)
                {
                    fprintf(stdout, "Iterate -> %d\n", i);
                    write(fd_ctp[WRITE], msg, sizeof(msg));
                    fprintf(stdout, "Child  wrote -> %s\n", msg);
                    sleep(1);
                    read(fd_ptc[READ], c_buf, sizeof(c_buf));
                    fprintf(stdout, "Child readed: %s\n", c_buf);
                    memset(c_buf, 0, sizeof(c_buf));
                }

                close(fd_ctp[WRITE]);
                close(fd_ptc[READ]);
                fprintf(stdout, "SHUTDOWN CHILD\n");
                exit(0);
            default:
                fprintf(stdout, "PARENT process - %d\n", getpid());
                close(fd_ctp[WRITE]);
                close(fd_ptc[READ]);

                while (read(fd_ctp[READ], p_buf, sizeof(p_buf)) > 0)
                {
                    fprintf(stdout, "Parent readed: %s\n", p_buf);
                    memset(p_buf, 0, sizeof(p_buf));
                    write(fd_ptc[WRITE], answ, sizeof(answ));
                    fprintf(stdout, "Parent Wrote -> %s\n", answ);
                }

                close(fd_ctp[READ]);
                close(fd_ptc[WRITE]);
                fprintf(stdout, "SHUTDOWN PARENT\n");

        }
    }
    else
        return 1;

    return 0;
}
