#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    // setting exec argument vector
    char *exec_argv[MAXARG];
    int i = 0;
    for (; i < argc - 1; ++i) {
        exec_argv[i] = argv[i + 1];
    }

    // read from fd 0
    char buf[512];
    int j = 0;
    while(read(0, &buf[j], 1) == 1) {
        if (buf[j] == '\n') {
            buf[j] = 0;
            // append argument
            int index = i;
            exec_argv[index] = buf;
            exec_argv[index + 1] = 0;
            if (fork() == 0) {
                exec(exec_argv[0], exec_argv);
                fprintf(2, "exec  %s failed.\n", exec_argv[0]);
                exit(1);
            } else {
                wait(0);
            }
            j = 0;
        } else {
            j++;
        } 
    }
    exit(0);
}