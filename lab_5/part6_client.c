#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define SERVER_QUEUE "/var/tmp/server_q"


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Not provide queue or client id");
        exit(1);
    }
    int status_cr = creat(argv[1], 0600);
    if (status_cr == -1)
    {
        perror("Error while creating file");
        exit(1);
    }

    key_t keyq = ftok(argv[1], 42);
    key_t serverq = ftok(SERVER_QUEUE, 100);

    int serverqid = msgget(serverq, 0600);
    int msgqid = msgget(keyq, IPC_CREAT | 0600);
    fprintf(stdout, "Id server queue %d\n", serverqid);
    fprintf(stdout, "Id client queue %d\n", msgqid);
    if (msgqid == -1 || serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }
    struct msg_buf request, response;

    request.mtype = msgqid;
    sprintf(request.mtext, "Hello, world from %s\n", argv[1]);
    int send_status = msgsnd(serverqid, &request, strlen(request.mtext) + 1, 0);
    if (send_status == -1)
    {
        perror("Error occured while delivery message");
        exit(1);
    }

    ssize_t mbytes = msgrcv(msgqid, &response, sizeof(response.mtext), 0, 0);
    if (mbytes == -1)
    {
        perror("Error occured while receive message");
        exit(1);
    }

    fprintf(stdout, "Receive response\n%s", response.mtext);

    msgctl(msgqid, IPC_RMID, 0);
    unlink(argv[1]);

    return 0;
}
