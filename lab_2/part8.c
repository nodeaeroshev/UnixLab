#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


int main()
{
    pid_t pid;
    char info[] = "\tPID\tPPID\tPGID\n";
    char buffer[512];
    int fd, fd_child, fd_parent, c_rbytes, p_rbytes, status;

    fd = open("/Users/artem/UnixLab/lab_2/big_file", O_RDONLY);
    pid = fork();


    switch(pid)
    {
        case -1:
            perror("Fork failed!");
            exit(1);
        case 0:
            printf("%s", info);
            printf("CHILD:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            fd_child = creat("/Users/artem/UnixLab/lab_2/child_file", 0777);
            c_rbytes = read(fd, buffer, sizeof(buffer));
            write(fd_child, buffer, c_rbytes);
            close(fd_child);
            // printf("Text from child process:\n%s\n\n", buffer);
            printf("Child process finished\n");
            exit(0);
        default:
            printf("%s", info);
            printf("PARENT:\t%d\t%d\t%d\n\n", getpid(), getppid(), getpgid(getpid()));
            fd_parent = creat("/Users/artem/UnixLab/lab_2/parent_file", 0777);
            p_rbytes = read(fd, buffer, sizeof(buffer));
            write(fd_parent, buffer, p_rbytes);
            close(fd_parent);
            // printf("Text from parent process:\n%s\n\n", buffer);
            wait(&status);
    }

    close(fd);

    fd_child = open("/Users/artem/UnixLab/lab_2/child_file", O_RDONLY);
    c_rbytes = read(fd_child, buffer, sizeof(buffer));
    printf("Text from child file:\n%s\n\n", buffer);
    close(fd_child);

    fd_parent = open("/Users/artem/UnixLab/lab_2/parent_file", O_RDONLY);
    p_rbytes = read(fd_parent, buffer, sizeof(buffer));
    printf("Text from parent file:\n%s\n\n", buffer);
    close(fd_parent);

    printf("Parent process finished\n");

    return 0;
}
