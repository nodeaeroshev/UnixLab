#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Not provide queue");
        exit(1);
    }

    key_t keyq = ftok(argv[1], 42);
    key_t serverq = ftok("/var/tmp/server_q", 42);

    int serverqid = msgget(serverq, 0600);
    int msgqid = msgget(keyq, IPC_CREAT | 0600);
    if (msgqid == -1 || serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }

    struct msg_buf request, response;

    request.mtype = msgqid;
    sprintf(request.mtext, "Hello, world! From client %s\n", argv[1]);
    msgsnd(msgqid, &request, sizeof(request.mtext), 0);

    ssize_t mbytes = msgrcv(serverqid, &response, sizeof(response.mtext), msgqid, 0);
    if (mbytes == -1)
    {
        perror("Error occured while receive message");
        exit(1);
    }

    fprintf(stdout, "Readed msg from server:\n%s", response.mtext);

    return 0;
}
