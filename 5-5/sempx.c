#include <stdio.h>                                                              
#include <string.h>                                                             
#include <semaphore.h>                                                          
#include <unistd.h>                                                             
#include <fcntl.h>                                                              
#include <sys/errno.h>                                                          
                                                                                
const static char* SEM_NAME = "/test.sem";                                      
                                                                                
int main() {                                                                    
    sem_t* id = sem_open(SEM_NAME, O_CREAT, 0666, 66);                          
    if (id == SEM_FAILED) {                                                     
        perror("Erorr:");                                                       
        return 0;                                                               
    }                                                                           
                                                                                
    // remove the queue                                                         
    //mq_unlink(SEM_NAME);                                                      
    //mq_close(id);                                                             
                                                                                
    return 0;                                                                   
}
