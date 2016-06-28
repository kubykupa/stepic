#include <stdio.h>                                                                                                                      
#include <string.h>                                                                                                                     
#include <sys/ipc.h>                                                                                                                    
#include <sys/msg.h>                                                                                                                    
#include <sys/errno.h>                                                                                                                  
                                                                                                                                        
const static char* OUT_FILE = "/home/box/message.txt";                                                                                  
const static char* MSG_KEY = "/tmp/msg.temp";                                                                                           
                                                                                                                                        
#define MSG_PERM 0666                                                                                                                   
                                                                                                                                        
struct message {                                                                                                                        
    long mtype;                                                                                                                         
    char mtext[80];                                                                                                                     
};                                                                                                                                      
                                                                                                                                        
                                                                                                                                        
int main() {                                                                                                                            
    key_t key = ftok(MSG_KEY, 1);                                                                                                       
    int msgqid = msgget(key, IPC_CREAT | MSG_PERM);                                                                                     
                                                                                                                                        
    // read the message from queue                                                                                                      
    struct message msg;                                                                                                                 
    int rc = msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0);                                                                             
    if (rc < 0) {                                                                                                                       
        perror( strerror(errno) );                                                                                                      
        printf("msgrcv failed, rc=%d\n", rc);                                                                                           
        return 1;                                                                                                                       
    }                                                                                                                                   
                                                                                                                                        
    msg.mtext[rc]='\0';                                                                                                                 
    printf("received msg: %d %s\n",rc, msg.mtext);                                                                                      
    FILE* out = fopen(OUT_FILE, "w");                                                                                                   
    if (out == NULL) {                                                                                                                  
        printf("open file fail");                                                                                                       
    }                                                                                                                                   
                                                                                                                                        
    // fputs(msg.mtext, out);                                                                                                           
    fprintf(out, "%s\n", msg.mtext);                                                                                                    
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
