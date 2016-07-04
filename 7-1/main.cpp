#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

// g++ -o run main.cpp -lpthread
const static char* OUT_FILE = "/home/box/main.pid";

void* job(void *args) {
    unsigned int pid = getpid();
    printf("Hello from thread! pid: %d\n", pid);
    FILE* out = fopen(OUT_FILE, "w");
    fprintf(out, "%d", pid);
    fclose(out);
    sleep(5);
    return SUCCESS;
}

int main() {
    pthread_t thread;
    int status_addr;

    int status = pthread_create(&thread, NULL, job, NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }

    status = pthread_join(thread, (void**)&status_addr);
    if (status != SUCCESS) {
        printf("main error: can't join thread, status = %d\n", status);
        exit(ERROR_JOIN_THREAD);
    }
    printf("close");
    return 0;
}
