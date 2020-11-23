#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void redirect(int k, int p[]){
    close(k);
    dup(p[k]);
    close(p[0]);
    close(p[1]);
}

int main()
{
    int p[2];
    pipe(p);
    if(fork()== 0){
        redirect(0, p);
        char s[9];
        while(read(0, s, 1))printf("%s", s);
        close(0);
        fprintf(2, "\n1 killed\n");
        exit(0);
    }
    //redirect(1, p);
    if(fork() == 0){
        redirect(1, p);
        write(1, "asdf", 4);
        close(1);
        fprintf(2, "2 killed\n");
        exit(0);
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    wait(0);
    fprintf(2, "3 killed\n");
    exit(0);
}
