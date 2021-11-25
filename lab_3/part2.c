#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char str[] = "\nsignal SIGINT\n";


void sighandler(int sig)
{
    write(1, str, sizeof(str));
}

int main()
{
    // TODO В чём различие с signal
    struct sigaction handler;
    handler.sa_handler = sighandler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = SA_RESTART;

    sigaction(SIGINT, &handler, 0);

    getchar();

    return 0;
}
