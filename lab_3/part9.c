#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>


void signal_handler(int sig)
{
    fprintf(stdout, "ALARM: %d!\n", sig);
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
            // signal(SIGALRM, signal_handler);
            alarm(2);
            fprintf(stdout, "Child process -> %d\n", getpid());
            for (int i = 0; i < 100000000; i++)
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
            w_pid = wait(&status);
            fprintf(stdout, "Child process %d finished with status %d - %x\n", pid, status, status);
            exit(0);

    }
    return 0;
}
