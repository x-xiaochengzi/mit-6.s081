#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int p[]) __attribute__((noreturn));

void sieve(int pleft[])
{
    close(pleft[1]);
    int prime;
    if (read(pleft[0], &prime, sizeof(prime)) == 0) {
        close(pleft[0]);
        exit(0);
    }

    fprintf(1, "prime %d\n", prime);


    int pright[2];
    pipe(pright);
    if (fork() == 0) {
        sieve(pright);
    } else { // sieve
        close(pright[0]);
        int number = 0;
        while(read(pleft[0], &number, sizeof(number)) != 0) {
            if (number % prime != 0) {
                write(pright[1], &number, sizeof(number));
            }
        }
        close(pright[1]);
        close(pleft[0]);
    }
    wait(0);
    exit(0);
}



int
main(int argc, char *argv[]) 
{
    int p[2];
    pipe(p);

    if(fork() == 0) { // pipeline begins
        sieve(p);
    } else { // feeds the numbers 2-35 into the pipeline
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    wait(0);
    exit(0);
}