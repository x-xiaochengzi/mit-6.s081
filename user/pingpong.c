#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[]) 
{
    char buf[1];
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    if (fork() == 0) {
        // close
        close(p1[1]);
        close(p2[0]);
        // read from p1
        // no availabel data: wait
        read(p1[0], buf, 1);
        close(p1[0]);
        fprintf(1, "%d: received ping\n", getpid());
        // write to p2
        write(p2[1], "c", 1);
        close(p2[1]);
    } else {
        close(p1[0]);
        close(p2[1]);
        // write to p1
        write(p1[1], "p", 1);
        close(p1[1]);
        // read from p2
        read(p2[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p2[1]);
    }
    exit(0);
}