// Simple primes, use pipes and forks to create a chain of processes to filter out prime numbers.
// The parent process should send the numbers 2..35 to the first child process.
// create one process that reads from its left neighbor over a pipe and writes to its right neighbor over another pipe.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];

    if(pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    for(int num = 2; num <= 35; num++) {
        write(p[1], &num, sizeof(int)); // write source to pipe
    }

    int pid = fork();
    if(pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    } else if(pid == 0) {
        int prime;
        int num;
        int next_p[2];

        if (pipe(next_p) < 0) {
            fprintf(2, "pipe failed\n");
            exit(1);
        }
        // child process
        close(0);   // close stdin fd 0
        dup(p[0]);  // read data from pipe
        read(p[0], &prime, sizeof(int)); //read first data as prime
        printf("prime %d\n, prime"); // print prime

        close(p[0]); // close parent->chlid pipe read port

        while (read(p[0], &num, sizeof(int) > 0)) {
            if (num % prime != 0) {
                write(next_p[1], &num, sizeof(int));
            }
        }

        close(next_p[0]);
        close(next_p[1]);
    }


    close(p[0]); // shutdown read port
    exit(0);
}