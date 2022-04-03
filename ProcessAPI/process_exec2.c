#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
fork()와 exec()을 구분한 이유는? 확장성 제공을 위해서이다.
fork를 호출하고 exec를 호출하기 전에 환경을 설정하고 다양한 기능을 준비할 수 있다.
exec()이 호출되기 전에 표준 출력 파일을 닫고, p4.output파일을 연다.
*/
int main(int argc, char *argv[]){
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc=fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){ //child(new process): redirect
        close(STDOUT_FILENO);
        open("./p4.output",O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        //now exec "wc"///
        char *myargs[3];
        myargs[0]=strdup("wc");
        myargs[1]=strdup("p3.c");
        myargs[2]=NULL;
        execvp(myargs[0],myargs);
    }else{
        int wc=wait(NULL);
    }
    return 0;
}