#include <stdio.h>
#include <assert.h>
#include <pthread.h>

//참고: gcc시 -pthread 옵션 붙이기

static volatile int counter = 0;

void *mythread(void *arg){
    printf("%s\n",(char *)arg);
    for(int i=0;i<1000000;i++){
        counter=counter+1;
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t p1, p2;
    int rc;
    printf("main: begin\n");
    rc=pthread_create(&p1, NULL, mythread, "A");
    assert(rc==0);
    rc=pthread_create(&p2, NULL, mythread, "B");
    assert(rc==0);
    //join waits for the threads to finish
    rc=pthread_join(p1, NULL);
    assert(rc==0);
    rc=pthread_join(p2, NULL);
    assert(rc==0);
    printf("main: end\n");
    printf("counter = %d\n",counter);
    return 0;
}