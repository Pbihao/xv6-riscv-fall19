#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int main(int argc, char const *argv[])
{
    int fd1[2];
    int fd2[2];
    char buff[4];
    if(pipe(fd1) != 0){
        printf("pipe() failed\n");
        exit(0);
    }
    if(pipe(fd2) != 0){
        printf("pipe() failed\n");
        exit(0);
    }
    if(fork() > 0){
        if(write(fd1[1], "ping", 4) != 4){
            printf("write failed");
            exit(0);
        }
        close(fd1[1]);
        if(read(fd2[0], buff, 4) == 4){
            printf("%d: received %s\n", getpid, buff);
            close(fd2[0]);
        }
    }else{
        if(read(fd1[0], buff, 4) == 4){
            close(fd1[0]);
            printf("%d: received %s\n", getpid(), buff);
            if(write(fd2[1], "pong", 4) != 4){
                printf("write failed");
                exit(0);
            }
            close(fd2[1]);
        }
    }
    exit(0);
}
