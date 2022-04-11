#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#define NUMCPUS 4

typedef struct __counter_t{
    int global; //global count
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;    
}counter_t;

//init: record threshold, init locks, init values
//  of all local counts and global count
void init(counter_t *c, int threshold){
    c->threshold=threshold;
    c->global=0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for(i=0;i<NUMCPUS;i++){
        c->local[i]=0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

//update: usually, just grab local lock and update local amount
//  once local count has risen by 'threshold', grab global
//  lock and transfer local values to it
void update(counter_t *c, int threadID, int amt){
    int cpu=threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu]+=amt;
    if(c->local[cpu]>=c->threshold){
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu]=0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

//get: just return global amount(which may not be perfect)
int get(counter_t *c){
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}

int main(){
    counter_t c;
    counter_t c2;
    // 뮤텍스 객체 초기화, 기본 특성으로 초기화 했음
    init(&c, 4);
    update(&c, 0, 1);
    update(&c, 0, 1);
    update(&c, 0, 1);
    update(&c, 0, 1);
    update(&c, 0, 1);
    update(&c, 0, 1);
    printf("%d\n",get(&c));
}