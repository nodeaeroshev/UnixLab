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
    unsigned int spgrp;

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed!");
            exit(1);
        case 0:
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            spgrp = setpgrp();
            printf("NEW SPGRP!\n");
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            pause();
            printf("Child process finished\n");
            exit(0);
        default:
            printf("%s", info);
            printf("PARENT:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            pause();
            printf("Parent process finished\n\n");
            exit(0);
    }

    return 0;
}
