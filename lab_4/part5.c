#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


enum PipeAction
{
    READ,
    WRITE
};


int main()
{
    int pipe_f[2];

    if (!pipe(pipe_f))
    {
        switch (fork())
        {
            case -1:
                perror("Fork failed");
                exit(1);
            case 0:
                close(pipe_f[READ]);
                fprintf(stdout, "CHILD process -> %d\n", getpid());

                if (dup2(pipe_f[WRITE], 1) == -1)
                {
                    perror("Error dup2 to stdin");
                    exit(1);
                }
                execlp("who", "who", NULL);

                close(pipe_f[WRITE]);
                exit(0);
            default:
                close(pipe_f[WRITE]);
                fprintf(stdout, "PARENT process -> %d\n", getpid());

                if (dup2(pipe_f[READ], 0) == -1)
                {
                    perror("Error dup2 to stdout");
                    exit(1);
                }
                execlp("wc", "wc", "-l", NULL);

                close(pipe_f[READ]);
                exit(0);
        }
    }

    return 0;
}
