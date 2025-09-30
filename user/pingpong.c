#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    char* sosal_message = "sosal?";
    char* yes_message = "da, kone4no";

    int p2c[2];
    int c2p[2];
    
    if (pipe(p2c) < 0 || pipe(c2p) < 0) {
        printf("essheeee razzz\n");
        exit(1);
    }

    int pid = fork();

    if (pid == 0) {
        close(p2c[1]);
        close(c2p[0]);
        char *buf = malloc(sizeof(char) * 100);
        int size = 0;
        for (;read(p2c[0], buf + size, 1) > 0;size++);
        buf[size] = 0;
        printf("%d: %s\n", getpid(), buf);
        close(p2c[0]);
        write(c2p[1], yes_message, strlen(yes_message));
        close(c2p[1]);
    } else {
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], sosal_message, strlen(sosal_message));
        close(p2c[1]);
        char *buf = malloc(sizeof(char) * 100);
        int size = 0;
        for (;read(c2p[0], buf + size, 1) > 0;size++);
        buf[size] = 0;
        printf("%d: got %s\n", getpid(), buf);
        close(c2p[0]);
    }

    exit(0);
}