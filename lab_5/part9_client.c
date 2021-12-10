#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define REQUEST_QUEUE "/var/tmp/request_q"
#define RESPONSE_QUEUE "/var/tmp/response_q"


struct msg_buf {
    long mtype;
    char mtext[64];
};


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Provide type request");
        exit(1);
    }
    int msg_type = strtol(argv[1], NULL, 10);

    key_t requestq = ftok(REQUEST_QUEUE, 1);
    key_t responseq = ftok(RESPONSE_QUEUE, 1);

    int requestqid = msgget(requestq, 0600);
    int responseqid = msgget(responseq, 0600);
    fprintf(stdout, "Id server queue %d\n", requestqid);
    fprintf(stdout, "Id client queue %d\n", responseqid);
    if (responseqid == -1 || requestqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }
    struct msg_buf request, response;

    request.mtype = msg_type;
    sprintf(request.mtext, "Hello, from client %d!\n", msg_type);
    int send_status = msgsnd(requestqid, &request, strlen(request.mtext) + 1, 0);
    if (send_status == -1)
    {
        perror("Error occured while delivery message");
        exit(1);
    }

    ssize_t mbytes = msgrcv(responseqid, &response, sizeof(response.mtext), msg_type, 0);
    if (mbytes == -1)
    {
        perror("Error occured while receive message");
        exit(1);
    }

    fprintf(stdout, "Receive response\n%s", response.mtext);

    return 0;
}
