#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    fork()를 통해서 자식 process를 만들었다.
    하지만 어떤 프로세스가 먼저 수행될 것인지는 알 수 없다.(Non-determinism)
*/
int main(int argc, char *argv[]){
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc=fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    }else{
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}