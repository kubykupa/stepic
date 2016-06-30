#include <stdio.h>                                                                                                           
#include <string.h>                                                                                                          
#include <sys/errno.h>                                                                                                       
#include <sys/mman.h>                                                                                                        
#include <unistd.h>                                                                                                          
#include <fcntl.h>                                                                                                           

const static char* MEM_NAME = "/test.shm";                                                                                   
const static int MEM_SIZE = 1024 * 1024;                                                                                     

int main() {                                                                                                                 
    int id = shm_open(MEM_NAME,  O_RDWR | O_CREAT | O_TRUNC, 0666);                                                          
    if (id == -1) {                                                                                                          
        perror("shm_open");                                                                                                  
        return 1;                                                                                                            
    }                                                                                                                        

    if (ftruncate(id, MEM_SIZE) == -1 ) {                                                                                    
        perror("ftruncate");                                                                                                 
        return 1;                                                                                                            
    }                                                                                                                        


    void* addr = mmap(0, MEM_SIZE, PROT_WRITE, MAP_SHARED, id, 0);                                                           
    if ( addr == (void*) -1 ) {                                                                                              
        perror("mmap");                                                                                                      
        return 1;                                                                                                            
    }                                                                                                                        

    memset(addr, 13, MEM_SIZE);                                                                                              
    munmap(addr, MEM_SIZE);                                                                                                  

    return 0;                                                                                                                
}        

