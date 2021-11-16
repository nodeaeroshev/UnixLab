#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


void fs_max(long int *fs_array, int arr_size, char *fn_array[])
{
    long int max_size = 0;
    char *name;

    for (int i = 0; i < arr_size; i++)
    {
        if (fs_array[i] > max_size)
        {
            max_size = fs_array[i];
            name = fn_array[i + 1];
        }
    }
    printf("Max size file: '%s' %ld bytes\n", name, max_size);
}


int main(int argc, char *argv[])
{
    int status;
    int array_size = argc - 1;
    long int f_size[array_size];
    struct stat file_stat;

    for (int i = 0; i < array_size; i++)
    {
        status = stat(argv[i + 1], &file_stat);
        if (status == 0)
            f_size[i] = file_stat.st_size;
        else
            printf("Error while getting stat file: %s\n", argv[i + 1]);
    }
    fs_max(f_size, array_size, argv);

    return 0;
}
