#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>


void child_handler(int sig)
{
    int status;
    pid_t pid;

    while((pid = waitpid(-1, &status, WNOHANG)) != -1)
    {
        if (WIFEXITED(pid))
        {
            int exit_status = WEXITSTATUS(status);
            fprintf(stdout, "Exit status of the child %d was %d\n", status, exit_status); 
        }
    }
}


int main()
{
    struct sigaction act;
    act.sa_handler = child_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;

    sigaction(SIGCHLD, &act, 0);

    for (int i = 0; i < 10; i++)
    {
        if (fork() == 0)
        {
            fprintf(stdout, "Child pid -> %d\n", getpid());
            sleep(5);
            exit(0);
        }
    }

    while(1) pause();

    return 0;
}
