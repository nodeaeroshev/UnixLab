#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define REQUEST_QUEUE "/var/tmp/request_q"
#define RESPONSE_QUEUE "/var/tmp/response_q"

int requestqid = 0, responseqid = 0;


void msg_handler(int sig)
{
    msgctl(requestqid, IPC_RMID, 0);
    msgctl(responseqid, IPC_RMID, 0);
    unlink(REQUEST_QUEUE);
    unlink(RESPONSE_QUEUE);
    exit(0);
}


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main()
{
    int status_cr = creat(REQUEST_QUEUE, 0600);
    if (status_cr == -1)
    {
        perror("Error while creating file");
        exit(1);
    }
    status_cr = creat(RESPONSE_QUEUE, 0600);
    if (status_cr == -1)
    {
        perror("Error while creating file");
        exit(1);
    }

    key_t requestq = ftok(REQUEST_QUEUE, 1);
    key_t responseq = ftok(RESPONSE_QUEUE, 1);

    requestqid = msgget(requestq, IPC_CREAT | 0600);
    responseqid = msgget(responseq, IPC_CREAT | 0600);
    if (requestqid == -1 || responseqid == -1)
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
        mbytes = msgrcv(requestqid, &request, sizeof(request.mtext), 0, 0);
        if (mbytes == -1)
        {
            perror("Error occured while receive message");
            exit(1);
        }
        if (mbytes == 0)
            continue;

        fprintf(stdout, "Receive msg\n%s", request.mtext);

        response.mtype = request.mtype;
        send_status = msgsnd(responseqid, &response, strlen(response.mtext) + 1, 0);
        if (send_status == -1)
        {
            perror("Error occured while delivery message");
            exit(1);
        }

        memset(request.mtext, 0, sizeof(request.mtext));
    }

    return 0;
}
