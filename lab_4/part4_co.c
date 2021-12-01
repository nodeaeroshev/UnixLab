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


int main(int argc, char *argv[])
{
    char msg[] = "Hello, world!\n";

    fprintf(stdout, "CHILD process - %d\n", getpid());

    if (argc < 1)
        exit(1);

    int pipe_write = *argv[0] - '0';

    for (int i = 0; i < 10; i++)
    {
        write(pipe_write, msg, sizeof(msg));
        fprintf(stdout, "%d: Wrote -> %s\n", i, msg);
        sleep(1);
    }

    close(pipe_write);

    fprintf(stdout, "SHUTDOWN CHILD\n");

    return 0;
}
