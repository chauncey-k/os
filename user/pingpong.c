// Simple pingpong. 'ping-pong' a byte between two processes over a pair of pipes, one for each direction.
// The parent should send a byte to the child; the child should print "<pid>: received ping", where <pid> is its process ID, 
// write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, print "<pid>: received pong", and exit.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    if(pipe(p) < 0){
        // 创建失败
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    char byte = 'A';
    write(p[1], &byte, 1); // 向管道写入一个字节
    int pid = fork();

    if(pid < 0){
        // 创建子进程失败
        fprintf(2, "fork failed\n");
        exit(1);
    } else if(pid == 0){
        // 子进程
        read(p[0], &byte, 1); // 从管道读取一个字节
        write(p[1], &byte, 1); // 向管道写入一个字节
        printf("%d: received ping\n", getpid());
        close(p[0]);
        close(p[1]);
    }
    else{
        // 父进程
        wait(0); // 等待子进程结束
        read(p[0], &byte, 1); // 从管道读取一个字节
        printf("%d: received pong\n", getpid());
        close(p[0]);
        close(p[1]);
    }

    exit(0);
}