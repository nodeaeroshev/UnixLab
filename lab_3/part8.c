#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int counter = 100000000;


void signal_handler(int sig)
{
    fprintf(stdout, "Handler signal: %d\n", sig);
    counter = 0;
}


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
            signal(SIGUSR1, signal_handler);
            fprintf(stdout, "Child process -> %d\n", getpid());

            for (int i = 0; i < counter; i++)
            {
                fprintf(stdout, "outer loop: i = %d\n", i);
                for (int j = 0; j < 100000000; j++)
                {
                    continue;
                }
            }
            exit(0);
        default:
            fprintf(stdout, "Parent process -> %d\n", getpid());
            sleep(5);
            kill(pid, SIGUSR1);
            w_pid = wait(&status);
            if (WIFSIGNALED(status))
                fprintf(stdout, "Child process %d finished with status %d\n", pid, WTERMSIG(status));
            if (WIFEXITED(status))
                fprintf(stdout, "Good!\n");
            exit(0);

    }
    return 0;
}
