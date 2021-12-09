#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#define SERVER_QUEUE "/var/tmp/server_q"


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main()
{
    int status_cr = creat(SERVER_QUEUE, 0600);
    if (status_cr == -1)
    {
        perror("Error while creating file");
        exit(1);
    }
    key_t serverq = ftok(SERVER_QUEUE, 100);

    int serverqid = msgget(serverq, IPC_CREAT | 0600);
    if (serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }

    struct msg_buf request, response;
    ssize_t mbytes;
    int send_status;

    strcpy(response.mtext, "Hello from server!\n");
    while(1)
    {
        mbytes = msgrcv(serverqid, &request, sizeof(request.mtext), 0, 0);
        if (mbytes == -1)
        {
            perror("Error occured while receive message");
            exit(1);
        }
        if (mbytes == 0)
            continue;

        fprintf(stdout, "Receive msg\n%s", request.mtext);

        send_status = msgsnd(request.mtype, &response, strlen(response.mtext) + 1, 0);
        if (send_status == -1)
        {
            perror("Error occured while delivery message");
            exit(1);
        }

        memset(request.mtext, 0, sizeof(request.mtext));
    }

    return 0;
}
