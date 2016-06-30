#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <sys/shm.h>

const static char* MEM_KEY = "/tmp/mem.temp";
const static int MEM_SIZE = 1024 * 1024;

int main() {
    key_t key = ftok(MEM_KEY, 1);

    int id = shmget(key, MEM_SIZE, IPC_CREAT | 0666);
    void* ptr = shmat(id, NULL, 0);
    if (ptr == (void*)-1) {
        perror("Error shmat");
        return 0;
    }

    memset(ptr, 42, MEM_SIZE);
    shmdt(ptr);

    return 0;
}
