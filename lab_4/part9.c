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
    int rbytes = 1, wbytes = 1;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    int fd = creat(filename, S_IRWXU);
    if (fd < 0)
    {
        perror("Failed create file");
        exit(1);
    }

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

            for (int i = 0; i < 4; i++)
            {
                lock.l_type = F_WRLCK;
                fcntl(c_fd, F_SETLKW, &lock);

                wbytes = write(c_fd, msg, sizeof(msg));
                fprintf(stdout, "Wrote -> %s\n", msg);
                if (wbytes < 0)
                {
                    perror("Error while wrote in file");
                    exit(1);
                }

                lock.l_type = F_UNLCK;
                fcntl(c_fd, F_SETLK, &lock);

                sleep(1);
            }
            close(c_fd);
            kill(getppid(), SIGUSR1);
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
                lock.l_type = F_RDLCK;
                fcntl(p_fd, F_SETLKW, &lock);

                rbytes = read(p_fd, buf, sizeof(buf));
                fprintf(stdout, "Readed -> %s\n", buf);
                memset(buf, 0, sizeof(buf));

                lock.l_type = F_UNLCK;
                fcntl(p_fd, F_SETLK, &lock);

                sleep(1);
            }
            close(p_fd);
            wait(NULL);
    }

    return 0;
}
