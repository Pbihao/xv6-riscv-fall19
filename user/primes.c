#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void prime_gene(int fd){
    int now, x, pfd[2];
top:
    if (read(fd, &now, sizeof now) != 4){
        exit();
    }
    printf("prime %d\n", now);
    pipe(pfd);
    if(fork() == 0){
        close(fd);
        close(pfd[1]);
        fd = pfd[0];
        goto top;
    }
    close(pfd[0]);
    while(1){
        if(read(fd, &x, 4) != 4){
            exit();
        }
        if(x % now){
            if(write(pfd[1], &x, 4) != 4){
                exit();
            }
        }
    }
    wait();
    exit();
}

int main(int argc, char const *argv[]){
    int p[2];
    pipe(p);
    for(int i  = 2; i <= 31; i++){
        if(write(p[1], &i, sizeof i) != 4){
            printf("write error\n");
            exit();
        }
    }
    close(p[1]);
    prime_gene(p[0]);
    wait();
    return 0;
}
