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
    char msg[] = "Hello, world!\n";
    char buf[16];

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
                fprintf(stdout, "CHILD process - %d\n", getpid());
                close(fd_ctp[READ]);

                for (int i = 0; i < 10; i++)
                {
                    write(fd_ctp[WRITE], msg, sizeof(msg));
                    fprintf(stdout, "%d: Wrote -> %s\n", i, msg);
                    sleep(1);
                }

                fprintf(stdout, "SHUTDOWN CHILD\n");
                exit(0);
            default:
                fprintf(stdout, "PARENT process - %d\n", getpid());
                close(fd_ctp[WRITE]);

                int wbytes;
                while((wbytes = read(fd_ctp[READ], buf, sizeof(buf))) != 0)
                {
                    read(fd_ctp[READ], buf, sizeof(buf));
                    fprintf(stdout, "Readed: %s\n", buf);
                    memset(buf, 0, sizeof(buf));
                }
                
                fprintf(stdout, "SHUTDOWN PARENT\n");

        }
    }
    else
        return 1;

    return 0;
}
