#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main()
{
    key_t serverq = ftok("/var/tmp/server_q", 42);

    int serverqid = msgget(serverq, IPC_CREAT | 0600);
    if (serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }

    struct msg_buf request, response;

    ssize_t mbytes;
    while(1)
    {
        for (int i = 1; i < 11; i++)
        {
            mbytes = msgrcv(serverqid, &request, sizeof(request.mtext), i, MSG_NOERROR | IPC_NOWAIT);
            if (mbytes == -1)
            {
                if (errno == ENOMSG)
                    continue;
                perror("Error occured while receive message");
                exit(1);
            }
            fprintf(stdout, "Receive msg:\n%s", request.mtext);

            response.mtype = i;
            strcpy(response.mtext, "Hello from server!");
            msgsnd(i, &response, sizeof(response.mtext), 0);
        }
    }

    return 0;
}
