#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


struct msg_buf {
    long mtype;
    char mtext[32];
};

int main()
{
    key_t keyq = ftok("progile_q", 42);
    int msgqid = msgget(keyq, IPC_CREAT | 0600);
    if (msgqid == -1)
    {
        perror("Failed create queue");
        exit(1);
    }

    struct msg_buf msg;

    msg.mtype = 1;
    strcpy(msg.mtext, "Hello, world!");
    msgsnd(msgqid, &msg, sizeof(msg), 0);
    memset(msg.mtext, 0, sizeof(msg.mtext));

    msg.mtype = 2;
    strcpy(msg.mtext, "Hello, Linux!");
    msgsnd(msgqid, &msg, sizeof(msg), 0);
    memset(msg.mtext, 0, sizeof(msg.mtext)); 

    msg.mtype = 3;
    strcpy(msg.mtext, "Hello, Python!");
    msgsnd(msgqid, &msg, sizeof(msg), 0);
    memset(msg.mtext, 0, sizeof(msg.mtext)); 

    struct msqid_ds ds;
    msgctl(msgqid,IPC_STAT,&ds);
    fprintf(stdout, "Owner's UID: %d\n", ds.msg_perm.uid);
    fprintf(stdout, "Owner's GID: %d\n", ds.msg_perm.gid);
    fprintf(stdout, "Message stime: %ld\n", ds.msg_stime);
    fprintf(stdout, "Message rtime: %ld\n", ds.msg_rtime);
    fprintf(stdout, "Message ctime: %ld\n", ds.msg_ctime);
    fprintf(stdout, "Number of messages: %lu\n", ds.msg_qnum);
    fprintf(stdout, "Maximum number of bytes: %lu\n", ds.msg_qbytes);

    return 0;
}
