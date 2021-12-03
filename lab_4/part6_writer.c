#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>


#define FIFO_PATH "/tmp/fifo_pipe"

void handler(int sig)
{
    fprintf(stdout, "handler SIGPIPE");
    exit(1);
}


int main()
{
    signal(SIGPIPE, handler);
    int status = mkfifo(FIFO_PATH, 0600); // то же самое
    // int status = mknod(FIFO_PATH, 0600, S_IFIFO);
    if (status == -1)
    {
        if (errno != EEXIST)
        {
            perror("Failed created FIFO");
            exit(1);
        }
    }

    int f_fd = open(FIFO_PATH, O_WRONLY | O_NDELAY); // Открытие в блокирующем режиме
    if (f_fd == -1)
    {
        perror("Failed opened FIFO");
        exit(1);
    }

    char msg[] = "Hello, world!";
    for (int i = 0; i < 10; i++)
    {
        fprintf(stdout, "%d: Wrote -> %s\n", i, msg);
        write(f_fd, msg, sizeof(msg));
    }

    close(f_fd);
    return 0;
}
