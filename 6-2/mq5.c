#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

const static char* OUT_FILE = "./message.txt";
const static char* MSG_KEY = "/tmp/msg.temp";

struct message {
    long mtype;
    char mtext[80];
};


int main() {
    key_t key = ftok(MSG_KEY, 0666);
    int msgqid = msgget(key, IPC_CREAT);

    // read the message from queue
    struct message msg;
    int rc = msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0);
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgrcv failed, rc=%d\n", rc);
        return 1;
    }

    msg.mtext[rc] = '\0';
    printf("received msg: %s\n", msg.mtext);
    FILE* out = popen(OUT_FILE, "w");
    fputs(msg.mtext, out);
    fclose(out);

    // remove the queue
    rc = msgctl(msgqid, IPC_RMID, NULL);
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, rc=%d\n", rc);
        return 1;
    }
    printf("message queue %d is gone\n", msgqid);
    return 0;
}
