#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>


int serverqid = 0;

struct msg_buf {
    long mtype;
    char mtext[64];
};

void msg_handler(int sig)
{
    msgctl(serverqid, IPC_RMID, 0);
    exit(2);
}


int main()
{
    signal(SIGINT, msg_handler);
    key_t serverq = ftok("/var/tmp/server_q", 42);

    int serverqid = msgget(serverq, IPC_CREAT | 0600);
    if (serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }

    struct msg_buf request, response;
    ssize_t mbytes;

    strcpy(response.mtext, "Hello from server!\n");
    while(1)
    {
        mbytes = msgrcv(serverqid, &request, sizeof(request.mtext), 0, MSG_NOERROR | IPC_NOWAIT);
        if (mbytes == -1)
        {
            if (errno == ENOMSG)
                continue;

            perror("Error occured while receive message");
            exit(1);
        }
        if (mbytes == 0)
            continue;

        fprintf(stdout, "Receive msg\n%s", request.mtext);

        msgsnd(request.mtype, &response, strlen(response.mtext) + 1, 0);

        memset(request.mtext, 0, sizeof(request.mtext));
    }

    return 0;
}
