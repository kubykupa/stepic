#include <stdio.h>                                                                                              
#include <string.h>                                                                                             
#include <mqueue.h>                                                                                             
#include <sys/errno.h>                                                                                          
                                                                                                                
const static char* OUT_FILE = "/home/box/message.txt";                                                          
const static char* QUEUE_NAME = "/test.mq";                                                                     
                                                                                                                
static const size_t BUFFER_SIZE = 8192;                                                                         
                                                                                                                
int main() {                                                                                                    
   /* initialize the queue attributes */                                                                        
    struct mq_attr attr;                                                                                        
    attr.mq_flags = 0;                                                                                          
    attr.mq_maxmsg = 10;                                                                                        
    attr.mq_msgsize = BUFFER_SIZE;                                                                              
    attr.mq_curmsgs = 0;                                                                                        
                                                                                                                
    /* create the message queue */                                                                              
    mqd_t mq_id = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);                                         
    if (mq_id == -1) {                                                                                          
        printf("Bad queue descriptor");                                                                         
        return 0;                                                                                               
    }                                                                                                           
                                                                                                                
    // read the message from queue                                                                              
    char buffer[BUFFER_SIZE + 1];                                                                               
    ssize_t size =  mq_receive(mq_id, buffer, BUFFER_SIZE, NULL);                                               
    if (size < 0) {                                                                                             
        printf("recieve fail");                                                                                 
        return 0;                                                                                               
    }                                                                                                           
                                                                                                                
    buffer[size] = '\0';                                                                                        
    printf("received msg: %s\n", buffer);                                                                       
    FILE* out = fopen(OUT_FILE, "w");                                                                           
    fputs(buffer, out);                                                                                         
    fclose(out);                                                                                                
                                                                                                                
    // remove the queue                                                                                         
    mq_unlink(QUEUE_NAME);                                                                                      
    mq_close(mq_id);                                                                                            
    printf("message queue gone\n");                                                                             
                                                                                                                
    return 0;                                                                                                   
} 
