#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    if (argc < 2){
        printf("Please enter correct paramenter\n");
        exit(0);
    }
    printf("System will sleep %s times\n", argv[1]);
    sleep(atoi(argv[1]) * 10);
    printf("System sleep over~\n");
    exit(0);
}

