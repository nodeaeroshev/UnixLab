#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;
    int status_code = 0;

    printf("Started with pid -> %d\n", getpid());

    pid = fork();
    printf("Forked pid -> %d\n", pid);

    switch(pid)
    {
        case -1:
            perror("Fork failed!");
            status_code = 1;
        case 0:
            printf("Child with pid -> %d\n", getpid());
        default:
            printf("Parent with pid -> %d\n", getpid());
    }

    return status_code;
}
