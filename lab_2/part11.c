#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
    pid_t pid;
    int status;
    char *cmd = argv[1];
    argv += 1; // Сдвигаем, чтобы не захватило имя исполняемого файла от C

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed!");
            exit(1);
        case 0:
            printf("Child process!\n");
            execvp(cmd, argv);
            exit(0);
        default:
            printf("Parent process!\n");
            wait(&status);
            // system("ls -lha");
            exit(0);
    }

    return 0;
}
