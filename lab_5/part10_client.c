#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define SERVER_QUEUE "/var/tmp/s_server_q"


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main()
{
    key_t serverq = ftok(SERVER_QUEUE, 1);

    int serverqid = msgget(serverq, 0600);
    fprintf(stdout, "Id server queue %d\n", serverqid);
    if (serverqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }
    struct msg_buf request, response;

    request.mtype = 105;
    strcpy(request.mtext, "Hello, from client\n");
    int send_status = msgsnd(serverqid, &request, strlen(request.mtext) + 1, 0);
    if (send_status == -1)
    {
        perror("Error occured while delivery message");
        exit(1);
    }

    ssize_t mbytes = msgrcv(serverqid, &response, sizeof(response.mtext), 205, 0);
    if (mbytes == -1)
    {
        perror("Error occured while receive message");
        exit(1);
    }

    fprintf(stdout, "Receive response\n%s", response.mtext);

    return 0;
}
