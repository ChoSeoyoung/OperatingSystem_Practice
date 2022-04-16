#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

//basic node structure
typedef struct __node_t{
    int key;
    struct __node_t *next;
}node_t;

//basic list structure (one used per list)
typedef struct __list_t{
    node_t *head;
    pthread_mutex_t lock;
}list_t;

typedef struct __myarg_t {
	list_t *L;
    int name;
	int key;
} myarg_t;

void List_Init(list_t *L){
    L->head=NULL;
    pthread_mutex_init(&L->lock, NULL);
}

void *List_Insert(void *arg){
    myarg_t *m = (myarg_t *)arg;
    list_t *L = m->L;

    node_t *new = malloc(sizeof(node_t));
    if(new==NULL){
        perror("malloc");
        pthread_mutex_unlock(&L->lock);
        printf("error");
    }
    printf("Thread<%d> insert %d in linked list\n",m->name,m->key);
    new->key=m->key;
    
    //just lock critical section
    pthread_mutex_lock(&L->lock);
    new->next=L->head;
    L->head=new;
    pthread_mutex_unlock(&L->lock);
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

int main(){
    list_t l;
    List_Init(&l);

    pthread_t p_thread1, p_thread2;

    myarg_t args;
    args.L=&l;
    args.name=1;
    args.key=1;

    printf("main:start\n");
    pthread_create(&p_thread1,NULL,List_Insert,(void *)&args);
    pthread_join(p_thread1,NULL);
    printf("main:end\n");
}