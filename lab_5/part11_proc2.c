#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define SYNC_QUEUE "/var/tmp/sync_q"


struct msg_buf {
    long mtype;
    char mtext[32];
};


int main()
{
    int status_cr = creat(SYNC_QUEUE, 0600);
    if (status_cr == -1)
    {
        perror("Error while creating file");
        exit(1);
    }
    key_t syncq = ftok(SYNC_QUEUE, 1);

    int syncqid = msgget(syncq, IPC_CREAT | 0600);
    fprintf(stdout, "Id sync queue %d\n", syncqid);
    if (syncqid == -1)
    {
        perror("Failed create or get queue");
        exit(1);
    }
    struct msg_buf sync_s, sync_r;

    sync_s.mtype = 101;
    sprintf(sync_s.mtext, "Sync from proc %d\n", getpid());
    int send_status = msgsnd(syncqid, &sync_s, strlen(sync_s.mtext) + 1, 0);
    if (send_status == -1)
    {
        perror("Error occured while delivery message");
        exit(1);
    }

    ssize_t mbytes = msgrcv(syncqid, &sync_r, sizeof(sync_r.mtext), 100, 0);
    if (mbytes == -1)
    {
        perror("Error occured while receive message");
        exit(1);
    }

    fprintf(stdout, "Receive response\n%s", sync_r.mtext);

    msgctl(syncqid, IPC_RMID, 0);

    return 0;
}
