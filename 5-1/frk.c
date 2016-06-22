#include <unistd.h>
#include <stdio.h>
#include <iostream>


void f1() {
    int p0 = fork();
    int p1 = fork();
    int p2 = fork();
    int p3 = fork();
    printf("%d Hello! %d %d %d %d\n",getpid(), p0, p1, p2, p3);
}

void f2() {
    if(fork()) {
        if(fork()) {
            if(fork()) {
                fork();
            }
        }
    }
    printf("Hello!\n");
}

int main(int argc, char **argv) {
    /*f1();*/
    f2();
    return 0;
}
