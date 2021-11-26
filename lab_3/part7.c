#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>


// TODO
int main()
{
    pid_t pid, w_pid;
    int status;

    pid = fork();
    switch(pid)
    {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            fprintf(stdout, "Child process -> %d\n", getpid());
            pause();
            exit(0);
        default:
            fprintf(stdout, "Parent process -> %d\n", getpid());
            sleep(5);
            kill(pid, SIGUSR1);
            w_pid = wait(&status);
            if (WIFSIGNALED(status))
                fprintf(stdout, "Child process %d finished with status %d\n", pid, WTERMSIG(status));
            exit(0);

    }
    return 0;
}
