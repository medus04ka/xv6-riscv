#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
    int p2c[2];
    int c2p[2];

    char *ping_msg = "sosal&";
    char *pong_msg = "da<kone4no";

    if (pipe(p2c) < 0) {
        printf("pipe p2c failed\n");
        exit(1);
    }
    if (pipe(c2p) < 0) {
        printf("pipe c2p failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        close(p2c[1]);
        close(c2p[0]);

        char buf[100];
        int n = 0;
        int r;
        while ((r = read(p2c[0], buf + n, 1)) > 0) {
            n += r;
            if (n >= (int)sizeof(buf) - 1) break;
        }
        buf[n] = 0;

        printf("%d: %s\n", getpid(), buf);

        write(c2p[1], pong_msg, strlen(pong_msg));
        close(p2c[0]);
        close(c2p[1]);

        exit(0);
    } else {
        close(p2c[0]); 
        close(c2p[1]); 

        write(p2c[1], ping_msg, strlen(ping_msg));
        close(p2c[1]);

        char buf[100];
        int n = 0;
        int r;
        while ((r = read(c2p[0], buf + n, 1)) > 0) {
            n += r;
            if (n >= (int)sizeof(buf) - 1) break;
        }
        buf[n] = 0;

        printf("%d: %s\n", getpid(), buf);

        close(c2p[0]);

        wait(0);
    }

    exit(0);
}
