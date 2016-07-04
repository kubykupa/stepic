#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12

// g++ -o run main.cpp -lpthread
const static char* OUT_FILE = "/home/box/main.pid";
const static int DELAY = 5;

pthread_mutex_t mutex;
pthread_spinlock_t slock;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void save_pid() {
    unsigned int pid = getpid();
    printf("Job pid: %d\n", pid);
    FILE* out = fopen(OUT_FILE, "w");
    fprintf(out, "%d", pid);
    fclose(out);
}

void* wait_mutex(void *args) {
    unsigned int pid = getpid();
    printf("%d wait mutex\n", pid);
    pthread_mutex_lock( &mutex );

    sleep(DELAY);
    pthread_mutex_unlock( &mutex );
    printf("%d mutex unlocked\n", pid);
    return 0;
}

void* wait_spin(void *args) {
    unsigned int pid = getpid();
    printf("%d wait spin lock\n", pid);
    pthread_spin_lock( &slock );

    sleep(DELAY);
    pthread_spin_unlock( &slock );
    printf("%d spinlock unlocked\n", pid);

    return 0;
}

void* wait_read(void *args) {
    unsigned int pid = getpid();
    printf("%d wait rd lock\n", pid);
    pthread_rwlock_rdlock( &rwlock );

    sleep(DELAY);
    pthread_rwlock_unlock( &rwlock );
    printf("%d rd lock unlocked\n", pid);
    return 0;
}

void* wait_write(void *args) {
    unsigned int pid = getpid();
    printf("%d wait rw lock\n", pid);
    pthread_rwlock_wrlock( &rwlock );

    pthread_rwlock_unlock( &rwlock );
    printf("%d rw lock unlocked\n", pid);
    return 0;
}

void start_job(pthread_t* thread, void* (*func)(void*)) {
    int status = pthread_create(thread, NULL, func, NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
}

void wait_job(pthread_t thread) {
    int status = pthread_join(thread, NULL);
    if (status != 0) {
        printf("main error: can't join thread, status = %d\n", status);
        exit(ERROR_JOIN_THREAD);
    }
}

int main() {
    // init
    /*mutex = PTHREAD_MUTEX_INITIALIZER;*/
    pthread_spin_init(&slock, PTHREAD_PROCESS_SHARED);
    /*rwlock = PTHREAD_RWLOCK_INITIALIZER;*/

    save_pid();

    pthread_t p1, p2, p3, p4;
    int status_addr;

    // lock all
    pthread_mutex_lock( &mutex );
    pthread_spin_lock( &slock );
    pthread_rwlock_rdlock( &rwlock );

    // launch all
    start_job(&p1, wait_mutex);
    start_job(&p2, wait_spin);
    start_job(&p3, wait_read);
    start_job(&p4, wait_write);

    pthread_mutex_unlock( &mutex );
    pthread_spin_unlock( &slock );
    pthread_rwlock_unlock( &rwlock );

    // wait all complete
    wait_job(p1);
    wait_job(p2);
    wait_job(p3);
    wait_job(p4);

    // release  // Destroy.
    pthread_mutex_destroy(&mutex);
    pthread_spin_destroy(&slock);
    pthread_rwlock_destroy(&rwlock);

    printf("close");
    return 0;
}
