#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void sighandler(int sig)
{
    fprintf(stdout, "\nsignal SIGINT\n");
}


int main()
{
    // Sigaction настраивает надёжные сигнала
    struct sigaction handler;
    handler.sa_handler = sighandler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = SA_RESTART;

    sigaction(SIGINT, &handler, NULL);

    getchar();

    return 0;
}
