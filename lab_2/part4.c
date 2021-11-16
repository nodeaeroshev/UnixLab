#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;
    char info[] = "\tPID\tPPID\tPGID\n";

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed");
            return 1;
        case 0:
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            sleep(3);
            printf("\nNew parent -> %d\n", getppid());
            printf("Child process finished\n");
            return 0;
        default:
            printf("%s", info);
            printf("PARENT:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            printf("Parent process finished\n\n");
            return 0;
    }
}
