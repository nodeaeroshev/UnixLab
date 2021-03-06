#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid, child_pid;
    int status;
    char info[] = "\tPID\tPPID\tPGID\n";

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            exit(0);
        default:
            printf("%s", info);
            printf("PARENT:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid())); // Почему поменялся PID
            child_pid = wait(&status);
            printf("PARENT: child process pid: %d finished\n", WIFEXITED(child_pid));
            if (WIFEXITED(status))
                printf("Child exited with code: %d\n", WIFEXITED(status));
            else
                printf("Child terminated abnormally!\n");
            exit(0);
    }

    return 0;
}
