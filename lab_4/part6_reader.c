#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define FIFO_PATH "/tmp/fifo_pipe"


int main()
{
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

    int f_fd = open(FIFO_PATH, O_RDONLY | O_NDELAY); // Открытие в блокирующем режиме
    if (f_fd == -1)
    {
        perror("Failed opened FIFO");
        exit(1);
    }

    int rbytes, i = 0;
    char buf[16];
    while ((rbytes = read(f_fd, buf, sizeof(buf))) > 0)
    {
        fprintf(stdout, "%d: Readed -> %s\n", i++, buf);
        memset(buf, 0, sizeof(buf));
    }

    close(f_fd);
    return 0;
}
