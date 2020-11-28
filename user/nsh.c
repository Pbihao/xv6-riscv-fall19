#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

//找到第一个|的位置
char *find_redi(char*s){
    while(*s != '\0' && *s != '|')s++;
    if(*s=='|'){
        *s = '\0';
        return s+1;
    }return 0;
}


//重定向
void redirect(int k, int p[]){
    close(k);
    dup(p[k]);
    close(p[0]);
    close(p[1]);
}

int is_blank(char s){
    if(s == ' ' || s == '\n' || s == '<' || s == '>')return 1;
    return 0;
}

char cmd_buf[1024];
char* cmd1,*cmd2;



void handle(char *s){
    char buf[32][32];
    char *cmd[32];
    int cnt = -1, input = -1, output = -1, tot = 0;
    char *p = buf[0];
    for(char *c = s; *c; c++){
        if(*c == '<')input = cnt + 1;
        if(*c == '>')output = cnt + 1;
        if(!is_blank(*c) && (c == s || is_blank(*(c-1)))){
            cnt++;
            *p = '\0';
            p = buf[cnt];
        }
        if(!is_blank(*c))*p++ = *c;
    }
    if(input != -1){
        close(0);
        open(buf[input], O_RDONLY);
    }
    if(output != -1){
        close(1);
        //fprintf(2, "debug: %s\n", buf[output]);
        open(buf[output], O_WRONLY | O_CREATE);
    }
    for(int i = 0; i <= cnt; i++){
        if(i == input || i == output)continue;
        cmd[tot++] = buf[i];
    }
    cmd[tot] = 0;
        
    /*if(cmd[0][0] == 'w'){
        char cc[2];
        fprintf(2, "debug: wc:");
        while(read(0, cc, 1))fprintf(2, "%s", cc);
        fprintf(2, "\n");
    }else{
        //exec(cmd[0], cmd);
        write(1, "pbh", 3);
        fprintf(2, "debug: grep:");
    }*/
    if(fork()== 0){
        exec(cmd[0], cmd);
        exit(0);
    }
    wait(0);
}
//grep suggestions < README | wc > testsh.out
void work(){
    if(cmd1){
        int p[2];
        pipe(p);
        
        if(cmd2){
            if(fork() == 0){
                redirect(0, p);
                cmd1 = cmd2;
                cmd2 = find_redi(cmd1);
                work();
                close(0);
                exit(0);
            }
            redirect(1, p);
        }
        handle(cmd1);
        close(1);
        wait(0);
    }
    exit(0);
}

int main(){
    while(1){
        write(1, "@", 1);
        memset(cmd_buf, 0, 1024);
        gets(cmd_buf, 1024);
        if(cmd_buf[0] == 0)exit(0);

        *strchr(cmd_buf, '\n') = '\0';

        if(fork()==0){
            cmd1 = cmd_buf;
            cmd2 = find_redi(cmd_buf);
            work();
            sleep(100);
        }
        wait(0);
    }
    //sleep(100);
    exit(0);
}