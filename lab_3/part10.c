#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <signal.h>


void handler_ch_sig(int sig)
{
    fprintf(stdout, "\nSignal SIGCHLD\n");
    sleep(5);
}


void handler_int_sig(int sig)
{
    fprintf(stdout, "\nSignal SIGINT\n");
    sleep(5);
}


int main(void)
{
    struct sigaction act_ch, act_int;
    int status;
    pid_t pid;

    sigset_t mask;
    sigemptyset(&mask);
    // Если не поставить маску блокировки на свой же сигнал, то после обработки
    // сигнала будет обработан следующей в очереди сигнал
    sigaddset(&mask, SIGINT);  // Установили маску блокировки на SIGINT

    act_int.sa_handler = handler_int_sig;
    act_int.sa_mask = mask;
    act_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act_int, NULL);

    act_ch.sa_handler = handler_ch_sig;
    act_ch.sa_mask = mask;
    act_ch.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act_ch, NULL);

    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            fprintf(stdout, "CHILD process %d\n", getpid());
            sleep(1);
            exit(0);
        default:
            fprintf(stdout, "PARENT process %d\nignore Ctrl+C\n", getpid());

            sleep(5);
            wait(&status);
            fprintf(stdout, "Status CHILD %d\n", WIFEXITED(status));
            exit(0);
     }

     return 0;
}
