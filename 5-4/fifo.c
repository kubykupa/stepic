#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

static const char* IN_PATH = "./in.fifo";
static const char* OUT_PATH = "./out.fifo";
static const size_t BUFF_SIZE = 8192;

int main(int arc, char** argv) {
    char buffer[BUFF_SIZE];


    mkfifo(IN_PATH, 0666);
    mkfifo(OUT_PATH, 0666);

    FILE* f_in = fopen(IN_PATH, "r");
    FILE* f_out = fopen(OUT_PATH, "w");

    if (f_in == NULL || f_out == NULL) {
        printf("BAD pointers P");
        return 1;
    }

    while (1) {
        if ( fgets (buffer, BUFF_SIZE, f_in) == NULL ) {
            break;
        }
        fputs(buffer, f_out);

        printf("RES: [%s]", buffer);
    }
    fclose(f_in);
    fclose(f_out);
}
