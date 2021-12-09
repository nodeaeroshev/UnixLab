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


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		perror("Not provide queue and type");
		exit(1);
	}
	key_t keyq = ftok(argv[1], 42);
	long int msg_type = strtol(argv[2], NULL, 10);

	int msgqid = msgget(keyq, 0600);
    if (msgqid == -1)
    {
        perror("Failed create queue");
        exit(1);
    }

    fprintf(stdout, "Msgqid -> %d\n", msgqid);
    fprintf(stdout, "Msg type -> %ld\n", msg_type);

    struct msg_buf msg;
    ssize_t mbytes = msgrcv(msgqid, &msg, sizeof(msg.mtext), msg_type, MSG_NOERROR);
    if (mbytes == -1)
   	{
   		perror("Error occured while receive message");
   		exit(1);
   	}

   	fprintf(stdout, "Readed msg:\n%s\nType msg: %ld\n", msg.mtext, msg_type);

	return 0;
}
