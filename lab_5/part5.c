#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>



int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		perror("Not provide queue");
		exit(1);
	}
	key_t keyq = ftok(argv[1], 42);

	int msgqid = msgget(keyq, 0600);
    if (msgqid == -1)
    {
        perror("Failed create queue");
        exit(1);
    }

    fprintf(stdout, "Msgqid -> %d\n", msgqid);

    int status = msgctl(msgqid, IPC_RMID, 0);
    if (status == -1)
    {
        if (errno == EIDRM)
        {
            fprintf(stdout, "Queue %s, was deleted\n", argv[1]);
            exit(0);
        }
        perror("Error occured while deleting queue");
        exit(1);
    }

	return 0;
}
