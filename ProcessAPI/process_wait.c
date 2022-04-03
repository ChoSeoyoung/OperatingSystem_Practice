#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc=fork();
    if(rc<0){
        fprintf(stderr,"fork failed\n");
        exit(1);
    }else if(rc==0){ //child(new process)
        printf("hello, I am child (pid:%d)\n",(int)getpid());
    }else{ //parent의 수행을 child process의 수행이 끝난 뒤에 이루어지도록->Deterministic
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",rc,wc,(int)getpid());
    }

    return 0;
}