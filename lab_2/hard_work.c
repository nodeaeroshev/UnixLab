#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[], char *envp[])
{
    if (argc > 1)
    {
            printf("Number of args: %d\n", argc);
            printf("List of args:\n");
            int i = 0;
            while(argv[i] != NULL)
            {
                printf("%s, ", argv[i++]);
            }
            i = 0;
            printf("\nEnvironment:\n");
            while(envp[i] != NULL)
            {
                printf("%s, ", envp[i++]);
            }

        int result = atoi(argv[1]) + atoi(argv[2]);
        printf("\nResult -> %d\n", result);
    }
    return 0;
}
