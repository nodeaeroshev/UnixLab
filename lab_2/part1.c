#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

extern char **environ;


int main(int argc, char *argv[])
{
    char *var = "Hello, world!";

    int env = setenv("GREETING", var, 0);
    if (env != 0)
    {
        printf("Set env failed!\n");
        return 1;
    }
    printf("GREETING=%s\n", getenv("GREETING"));
    unsetenv("GREETING");

    while(*environ != NULL)
    {
        printf("%s\n", *environ);
        environ++;
    }
    return 0;
}
