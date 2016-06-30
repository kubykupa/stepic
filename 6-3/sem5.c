#include <stdio.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <sys/sem.h>

const static char* MSG_KEY = "/tmp/sem.temp";
const static int SEM_COUNT = 16;

int main() {
    key_t key = ftok(MSG_KEY, 1);
    int semid = semget(key, SEM_COUNT, IPC_CREAT | 0666);

    struct sembuf sops[SEM_COUNT];
    for (size_t i = 0; i < SEM_COUNT; ++i) {
        sops[i].sem_num = i;
        sops[i].sem_op = i;
        sops[i].sem_flg = 0;
    }
    semop(semid, sops, SEM_COUNT);
    perror("Error after semop: ");

    return 0;
}
