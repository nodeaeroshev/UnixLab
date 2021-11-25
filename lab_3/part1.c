#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char str[] = "\nsignal SIGINT\n";


void sighandler(int sig)
{
    // Здесь мы поменяли диспозицию процесса на сигнал
    write(1, str, sizeof(str));
    // printf не отправляет сразу в поток
    // Вернуть исходное поведение
    // signal(SIGINT, SIG_DFL);
}

int main()
{
    signal(SIGINT, sighandler); // Изменить поведение на сигнал

    getchar();

    return 0;
}
