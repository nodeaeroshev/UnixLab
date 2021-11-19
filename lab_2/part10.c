#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed!");
            exit(1);
        case 0:
            printf("\nChild process!\n");
            execl("/Users/artem/UnixLab/lab_2/hard_work.o", argv[1], argv[2], NULL);
            exit(0);
        default:
            printf("Parent process\n");
            printf("Number of args: %d\n", argc);
            printf("List of args:\n");
            int i = 0;
            while(argv[i] != NULL)
            {
                printf("%s, ", argv[i++]);
            }
            i = 0;
            printf("\nEnvironment:\n");
            while(envp[i] != NULL)
            {
                printf("%s, ", envp[i++]);
            }

    }

    return 0;
}
