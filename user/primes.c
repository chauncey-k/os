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
    int num = 2;

    if(pipe(p) < 0){
        // 创建失败
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    close(p[0]); // 关闭读端
    for(num = 2; num <= 35; num++){
        write(p[1], &num, sizeof(int)); // 向管道写入数字
    }

    int pid = fork();
    if(pid < 0){
        // 创建子进程失败
        fprintf(2, "fork failed\n");
        exit(1);
    } else if(pid == 0){
        int prime;
        // 子进程
        if (read(p[0], &prime, sizeof(int)) == 0);{ // 如果读取到最后一个数据
            close(p[0]);
            exit(0);

    }
    exit(0);
}