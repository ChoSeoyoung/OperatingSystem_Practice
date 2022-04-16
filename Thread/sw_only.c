#include <stdio.h>
#include <assert.h>
#include <pthread.h>

typedef sturct __lock_t {
    int flag;
}lock_t;

void init(lock_t *mutex){
    //0 -> lock is availale, 1->held
    mutex->flag=0; //현재 사용가능한 상태로 시작
}

void lock(lock_t *mutex){
    while(mutex->flag==1);
    //spin lock: 사용가능할 때까지 계속 확인하며 waiting
    mutex->flag=1;
}

void unlock(lock_t *mutex){
    mutex->flag=0;
}

int main(){
    
}