#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <signal.h>

int shared_counter = 0;


void ch_handler(int sig)
{
    fprintf(stdout, "Get parent signal\n");
    sleep(1);
}


void p_handler(int sig)
{
    fprintf(stdout, "Get child signal\n");
    sleep(1);
}


int main(void)
{
    pid_t ch_pid, p_pid;
    struct sigaction p_action, ch_action;
    sigset_t mask;

    // Настройка обработчика сигнала от родителя
    ch_action.sa_handler = ch_handler; // Обработчик сигнала
    ch_action.sa_flags = SA_NODEFER; // Настройка поведение при получении сигнала
    sigemptyset(&ch_action.sa_mask); // Очистка маски блокируемых сигналов
    sigaction(SIGUSR2, &ch_action, NULL); // Изменить диспозицию без сохранения старого поведения

    // Настройка обработчика сигнала от ребёнка
    p_action.sa_handler = p_handler;
    p_action.sa_flags = SA_NODEFER;
    sigemptyset(&p_action.sa_mask);
    sigaction(SIGUSR1, &p_action, NULL);

    // Настройка блокирующей маски
    sigfillset(&mask); // Заполняем маску всеми битами
    sigprocmask(SIG_BLOCK, &mask, NULL); // Устанавливаем блокировку на сигналы (1 - блокировать)

    ch_pid = fork();

    switch(ch_pid) 
    {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            p_pid = getppid();

            sigdelset(&mask, SIGUSR2); // Открываемся для сигнала SIGUSR2
       
            while(shared_counter != 5)
            {
                shared_counter += 1;
                kill(p_pid, SIGUSR1);
                sigsuspend(&mask); // Замена текущей маски блокирующей маской и ждём получения сигнала
            }

            exit(0);
        default:
            sigdelset(&mask, SIGUSR1); // Открываемся для сигнала SIGUSR1

            while(shared_counter != 5)
            {
                shared_counter += 1;
                sigsuspend(&mask); // Замена текущей маски блокирующей маской и ждём получения сигнала
                kill(ch_pid, SIGUSR2);
            }

            sleep(2);
            fprintf(stdout, "Done!\n");

            exit(0);
    }

    return 0;
}
