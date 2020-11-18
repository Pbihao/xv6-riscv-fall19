#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void prime_gene(int fd){
    int now, x, pfd[2];
top:

    if (read(fd, &now, sizeof now) != 4){
        close(pfd[0]);
        close(pfd[1]);
        close(fd);
        exit(0);
    }
    printf("prime %d\n", now);

    pipe(pfd);
    //printf("fd: %d", fd);
    while(1){
        if(read(fd, &x, 4) != 4){
          //  printf("debug: %d read finished\n", getpid());
            close(fd);
            close(pfd[1]);
            break;
        }
        if(x % now){
            if(write(pfd[1], &x, 4) != 4){
                printf("write failed to exit!!!!!");
                exit(0);
            }
        }
    }
    int id;
    if((id = fork()) == 0){
        //printf("fork a new -> %d and fd is %d;\n", getpid(), fd);
        fd = pfd[0];
        goto top;
    }else if(id < 0){
        printf("fork failed: %d\n", id);
        exit(0);
    }
    wait(0);
}

int main(int argc, char const *argv[]){
    int p[2];
    pipe(p);
    for(int i  = 2; i <= 31; i++){
        if(write(p[1], &i, sizeof i) != 4){
            printf("write error\n");
            exit(0);
        }
    }
    close(p[1]);
    prime_gene(p[0]);
    wait(0);
    exit(0);
}
