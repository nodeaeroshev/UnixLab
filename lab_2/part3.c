#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid, child_pid;
    int status_code = 0;
    int status;
    char info[] = "\tPID\tPPID\tPGID\n";

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed");
            status_code = 1;
        case 0:
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
        default:
            printf("%s", info);
            printf("PARENT:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            child_pid = wait(&status);
            printf("PARENT: child process pid: %d finished\n", child_pid);
            if (WIFEXITED(status))
                printf("Child exited with code: %d\n", WIFEXITED(status));
            else
                printf("Child terminated abnormally!\n");
    }

    return status_code;
}
