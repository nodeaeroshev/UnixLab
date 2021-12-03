#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include <signal.h>


int INFINITI_LOOP = 1;

void loop_handler(int sig)
{
    INFINITI_LOOP = 0;
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Didn't get file name");
        exit(1);
    }

    char *filename = argv[1];
    char msg[] = "Hello, world!";
    char buf[16];
    pid_t pid;
    int c_fd, p_fd;
    int rbytes, wbytes;

    creat(filename, S_IRWXU);
    pid = fork();

    switch(pid)
    {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            c_fd = open(filename, O_WRONLY);
            if (c_fd < 0)
            {
                perror("Error opening file");
                exit(1);
            }
            for (int i = 0; i < 100; i++)
            {
                wbytes = write(c_fd, msg, sizeof(msg));
                fprintf(stdout, "Wrote -> %s\n", msg);
                if (wbytes < 0)
                {
                    perror("Error while wrote in file");
                    exit(1);
                }
            }
            kill(getppid(), SIGUSR1);
            close(c_fd);
            exit(0);
        default:
            p_fd = open(filename, O_RDONLY);
            if (p_fd < 0)
            {
                perror("Error opening file");
                exit(1);
            }
            signal(SIGUSR1, loop_handler);

            while(INFINITI_LOOP)
            {
                rbytes = read(p_fd, buf, sizeof(buf));
                fprintf(stdout, "Readed -> %s\n", buf);
                memset(buf, 0, sizeof(buf));
                sleep(1);
            }
            close(p_fd);
            wait(NULL);
    }
}
