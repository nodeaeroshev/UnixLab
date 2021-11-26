#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>


int action_done = 0;


void child_handler(int sig)
{
    action_done = 1;
}


// TODO
int main()
{
    struct sigaction act;
    act.sa_handler = child_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;

    sigaction(SIGCHLD, &act, 0);

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
            if (action_done)
            {
                fprintf(stdout, "signal SIGCHLD\n");
                fprintf(stdout, "Child process %d finished with status %d - %x\n", pid, status, status);
            }
            exit(0);

    }
    return 0;
}
