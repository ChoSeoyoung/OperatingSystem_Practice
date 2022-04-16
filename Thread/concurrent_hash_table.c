#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#define BUCKETS 5

typedef struct __node_t{
    int key;
    struct __node_t *next;
}node_t;

typedef struct __list_t{
    node_t *head;
    pthread_mutex_t lock;
}list_t;

typedef struct __hash_t{
    list_t lists[BUCKETS];
}hash_t;

typedef struct __myarg_t {
	hash_t *H;
    int name;
	int key;
} myarg_t;

void List_Init(list_t *L){
    L->head=NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key, int name){
    node_t *new = malloc(sizeof(mode_t));
    if(new==NULL){
        perror("malloc");
        return -1;
    }
    printf("Thread<%d> insert %d in linked list\n",name,key);
    new->key=key;
    
    //just lock critical section
    pthread_mutex_lock(&L->lock);
    new->next=L->head;
    L->head=new;
    pthread_mutex_unlock(&L->lock);
    return 0;
}

int List_Lookup(list_t *L, int key){
    int rv=-1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while(curr){
        if(curr->key==key){
            rv=0;
            break;
        }
        curr=curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv;
}

void HASH_Init(hash_t *H){
    int i;
    for(i=0;i<BUCKETS;i++){
        List_Init(&H->lists[i]);
    }
}

void *Hash_Insert(void *arg){
    myarg_t *m = (myarg_t *)arg;
    hash_t *h = m->H;

    int bucket = m->key % BUCKETS;
    List_Insert(&h->lists[bucket],m->key,m->name);
}

int Hash_Lookup(hash_t *H, int key){
    int bucket = key % BUCKETS;
    return List_Lookup(&H->lists[bucket],key);
}

int main(){
    hash_t h;
    HASH_Init(&h);

    pthread_t p_thread1, p_thread2;
    myarg_t args;
    args.H=&h;
    args.name=1;
    args.key=1;

    printf("main:start\n");
    pthread_create(&p_thread1,NULL,Hash_Insert,(void *)&args);
    pthread_join(p_thread1,NULL);
    printf("main:end\n");
}