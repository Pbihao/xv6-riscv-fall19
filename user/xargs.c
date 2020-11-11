#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
char c;
int i = 0;
char buf[256];
int main(int argc, char *argv[])
{
    
    if (argc < 2){
        printf("Error input!\n");
        exit();
    }
    while(read(0, &c, 1) > 0){
        if(c != '\n'){
            buf[i++] = c;
            continue;
        }
        buf[i] = '\0';
        //printf("debug:%s\n", buf);
        //printf("debug:%s\n", argv[0]);
        //printf("debug:%s\n", argv[1]);
        //printf("debug:%s\n", argv[2]);
        //printf("debug:%s\n", argv[3]);
        i = 0;
        argv[argc] = buf;
        argv[argc + 1] = 0;
        if(fork()==0){
            exec(argv[1], argv + 1);
            exit();
        }else{
            wait();
        }
        buf[i] = '\0';
    }
    exit();
    return 0;
}
